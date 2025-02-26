/**
 *  Copyright Notice:
 *  Copyright 2021-2022 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

#include "internal/libspdm_requester_lib.h"

#if LIBSPDM_ENABLE_CAPABILITY_GET_CSR_CAP

/**
 * This function sends GET_CSR
 * to get csr from the device.
 *
 * @param[in]  context                      A pointer to the SPDM context.
 * @param[in]  session_id                   Indicates if it is a secured message protected via SPDM session.
 *                                          If session_id is NULL, it is a normal message.
 *                                          If session_id is NOT NULL, it is a secured message.
 * @param[in]  requester_info               requester info to gen CSR
 * @param[in]  requester_info_length        The len of requester info
 * @param[in]  opaque_data                  opaque data
 * @param[in]  opaque_data_length           The len of opaque data
 * @param[out] csr                          address to store CSR.
 * @param[out] csr_len                      on input, *csr_len indicates the max csr buffer size.
 *                                          on output, *csr_len indicates the actual csr buffer size.
 *
 * @retval RETURN_SUCCESS               The measurement is got successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
static libspdm_return_t libspdm_try_get_csr(libspdm_context_t *spdm_context,
                                            const uint32_t *session_id,
                                            void *requester_info, uint16_t requester_info_length,
                                            void *opaque_data, uint16_t opaque_data_length,
                                            void *csr, size_t *csr_len)
{
    libspdm_return_t status;
    spdm_get_csr_request_t *spdm_request;
    size_t spdm_request_size;
    spdm_csr_response_t *spdm_response;
    size_t spdm_response_size;
    size_t transport_header_size;
    uint8_t *message;
    size_t message_size;
    libspdm_session_info_t *session_info;
    libspdm_session_state_t session_state;

    if (!libspdm_is_capabilities_flag_supported(
            spdm_context, true, 0,
            SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CSR_CAP)) {
        return LIBSPDM_STATUS_UNSUPPORTED_CAP;
    }

    LIBSPDM_ASSERT(opaque_data_length < SPDM_MAX_OPAQUE_DATA_SIZE);

    if (spdm_context->connection_info.connection_state <
        LIBSPDM_CONNECTION_STATE_NEGOTIATED) {
        return LIBSPDM_STATUS_INVALID_STATE_LOCAL;
    }

    if (session_id != NULL) {
        session_info = libspdm_get_session_info_via_session_id(
            spdm_context, *session_id);
        if (session_info == NULL) {
            LIBSPDM_ASSERT(false);
            return LIBSPDM_STATUS_INVALID_STATE_LOCAL;
        }
        session_state = libspdm_secured_message_get_session_state(
            session_info->secured_message_context);
        if (session_state != LIBSPDM_SESSION_STATE_ESTABLISHED) {
            return LIBSPDM_STATUS_INVALID_STATE_LOCAL;
        }
    }

    transport_header_size = spdm_context->transport_get_header_size(spdm_context);
    status = libspdm_acquire_sender_buffer (spdm_context, &message_size, (void **)&message);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        return status;
    }
    LIBSPDM_ASSERT (message_size >= transport_header_size);
    spdm_request = (void *)(message + transport_header_size);
    spdm_request_size = message_size - transport_header_size;

    spdm_request->header.spdm_version = libspdm_get_connection_version (spdm_context);
    spdm_request->header.request_response_code = SPDM_GET_CSR;
    spdm_request->header.param1 = 0;
    spdm_request->header.param2 = 0;

    spdm_request->opaque_data_length = opaque_data_length;
    spdm_request->requester_info_length = requester_info_length;

    if (opaque_data_length != 0) {
        libspdm_copy_mem(spdm_request + 1,
                         spdm_request_size - sizeof(spdm_get_csr_request_t),
                         (uint8_t *)opaque_data, opaque_data_length);
    }

    if (requester_info_length != 0) {
        libspdm_copy_mem((uint8_t *)(spdm_request + 1) + opaque_data_length,
                         spdm_request_size - sizeof(spdm_get_csr_request_t),
                         (uint8_t *)requester_info, requester_info_length);
    }

    LIBSPDM_ASSERT(spdm_request->header.spdm_version >= SPDM_MESSAGE_VERSION_12);

    spdm_request_size = sizeof(spdm_get_csr_request_t) + opaque_data_length
                        + requester_info_length;

    status = libspdm_send_spdm_request(spdm_context, session_id, spdm_request_size, spdm_request);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        libspdm_release_sender_buffer (spdm_context);
        return status;
    }
    libspdm_release_sender_buffer (spdm_context);
    spdm_request = (void *)spdm_context->last_spdm_request;

    /* receive */
    status = libspdm_acquire_receiver_buffer (spdm_context, &message_size, (void **)&message);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        return status;
    }
    LIBSPDM_ASSERT (message_size >= transport_header_size);
    spdm_response = (void *)(message);
    spdm_response_size = message_size;

    libspdm_zero_mem(spdm_response, spdm_response_size);
    status = libspdm_receive_spdm_response(spdm_context, session_id,
                                           &spdm_response_size, (void **)&spdm_response);

    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        goto receive_done;
    }
    if (spdm_response_size < sizeof(spdm_message_header_t)) {
        status = LIBSPDM_STATUS_INVALID_MSG_SIZE;
        goto receive_done;
    }
    if (spdm_response->header.spdm_version != spdm_request->header.spdm_version) {
        status = LIBSPDM_STATUS_INVALID_MSG_FIELD;
        goto receive_done;
    }
    if (spdm_response->header.request_response_code == SPDM_ERROR) {
        status = libspdm_handle_error_response_main(
            spdm_context, session_id,
            &spdm_response_size,
            (void **)&spdm_response, SPDM_GET_CSR, SPDM_CSR);
        if (LIBSPDM_STATUS_IS_ERROR(status)) {
            goto receive_done;
        }
    } else if (spdm_response->header.request_response_code != SPDM_CSR) {
        status = LIBSPDM_STATUS_INVALID_MSG_FIELD;
        goto receive_done;
    }

    if (spdm_response->csr_length <= 0) {
        status = LIBSPDM_STATUS_INVALID_MSG_FIELD;
        goto receive_done;
    }

    if (*csr_len < spdm_response->csr_length) {
        *csr_len = spdm_response->csr_length;
        status =  LIBSPDM_STATUS_BUFFER_TOO_SMALL;
        goto receive_done;
    }

    libspdm_copy_mem(csr, *csr_len, spdm_response + 1, spdm_response->csr_length);
    *csr_len = spdm_response->csr_length;

    status = LIBSPDM_STATUS_SUCCESS;

receive_done:
    libspdm_release_receiver_buffer (spdm_context);
    return status;
}

libspdm_return_t libspdm_get_csr(void * spdm_context,
                                 const uint32_t *session_id,
                                 void * requester_info, uint16_t requester_info_length,
                                 void * opaque_data, uint16_t opaque_data_length,
                                 void *csr, size_t *csr_len)
{
    libspdm_context_t *context;
    size_t retry;
    uint64_t retry_delay_time;
    libspdm_return_t status;

    context = spdm_context;
    context->crypto_request = true;
    retry = context->retry_times;
    retry_delay_time = context->retry_delay_time;
    do {
        status = libspdm_try_get_csr(context, session_id,
                                     requester_info, requester_info_length,
                                     opaque_data, opaque_data_length,
                                     csr, csr_len);
        if ((status != LIBSPDM_STATUS_BUSY_PEER) || (retry == 0)) {
            return status;
        }

        libspdm_sleep(retry_delay_time);
    } while (retry-- != 0);

    return status;
}

#endif /*LIBSPDM_ENABLE_CAPABILITY_GET_CSR_CAP*/
