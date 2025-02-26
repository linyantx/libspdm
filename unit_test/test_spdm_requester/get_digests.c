/**
 *  Copyright Notice:
 *  Copyright 2021-2022 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

#include "spdm_unit_test.h"
#include "internal/libspdm_requester_lib.h"
#include "internal/libspdm_secured_message_lib.h"

#if LIBSPDM_ENABLE_CAPABILITY_CERT_CAP

static uint8_t m_libspdm_local_certificate_chain[LIBSPDM_MAX_CERT_CHAIN_SIZE];

static void *m_libspdm_local_certificate_chain_test_cert;
static size_t m_libspdm_local_certificate_chain_size;

static size_t m_libspdm_local_buffer_size;
static uint8_t m_libspdm_local_buffer[LIBSPDM_MAX_MESSAGE_M1M2_BUFFER_SIZE];

static bool m_get_digest;

static libspdm_return_t libspdm_requester_get_digests_test_send_message(
    void *spdm_context, size_t request_size, const void *request,
    uint64_t timeout)
{
    libspdm_test_context_t *spdm_test_context;

    spdm_test_context = libspdm_get_test_context();
    switch (spdm_test_context->case_id) {
    case 0x1:
        return LIBSPDM_STATUS_SEND_FAIL;
    case 0x2:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x3:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x4:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x5:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x6:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x7:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x8:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x9:
        return LIBSPDM_STATUS_SUCCESS;
    case 0xA:
        return LIBSPDM_STATUS_SUCCESS;
    case 0xB:
        return LIBSPDM_STATUS_SUCCESS;
    case 0xC:
        return LIBSPDM_STATUS_SUCCESS;
    case 0xD:
        return LIBSPDM_STATUS_SUCCESS;
    case 0xE:
        return LIBSPDM_STATUS_SUCCESS;
    case 0xF:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x10:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x11:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x12:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x13:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x14:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x15:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x16:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x17: {
        const uint8_t *ptr = (const uint8_t *)request;

        m_libspdm_local_buffer_size = 0;
        libspdm_copy_mem(m_libspdm_local_buffer, sizeof(m_libspdm_local_buffer),
                         &ptr[1], request_size - 1);
        m_libspdm_local_buffer_size += (request_size - 1);
    }
        return LIBSPDM_STATUS_SUCCESS;
    case 0x18:
        return LIBSPDM_STATUS_SUCCESS;
    case 0x19:
        return LIBSPDM_STATUS_SUCCESS;
    default:
        return LIBSPDM_STATUS_SEND_FAIL;
    }
}

static libspdm_return_t libspdm_requester_get_digests_test_receive_message(
    void *spdm_context, size_t *response_size,
    void **response, uint64_t timeout)
{
    libspdm_test_context_t *spdm_test_context;

    spdm_test_context = libspdm_get_test_context();
    switch (spdm_test_context->case_id) {
    case 0x1:
        return LIBSPDM_STATUS_RECEIVE_FAIL;

    case 0x2: {
        spdm_digest_response_t *spdm_response;
        uint8_t *digest;
        size_t spdm_response_size;
        size_t transport_header_size;

        ((libspdm_context_t *)spdm_context)
        ->connection_info.algorithm.base_hash_algo =
            m_libspdm_use_hash_algo;
        spdm_response_size = sizeof(spdm_digest_response_t) +
                             libspdm_get_hash_size(m_libspdm_use_hash_algo) * SPDM_MAX_SLOT_COUNT;
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_DIGESTS;
        spdm_response->header.param2 = 0;
        libspdm_set_mem(m_libspdm_local_certificate_chain,
                        sizeof(m_libspdm_local_certificate_chain),
                        (uint8_t)(0xFF));

        digest = (void *)(spdm_response + 1);
        libspdm_zero_mem (digest,
                          libspdm_get_hash_size(m_libspdm_use_hash_algo) * SPDM_MAX_SLOT_COUNT);
        libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                         sizeof(m_libspdm_local_certificate_chain), &digest[0]);
        spdm_response->header.param2 |= (0x01 << 0);

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response, response_size,
                                              response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x3: {
        spdm_digest_response_t *spdm_response;
        uint8_t *digest;
        size_t spdm_response_size;
        size_t transport_header_size;

        ((libspdm_context_t *)spdm_context)
        ->connection_info.algorithm.base_hash_algo =
            m_libspdm_use_hash_algo;
        spdm_response_size = sizeof(spdm_digest_response_t) +
                             libspdm_get_hash_size(m_libspdm_use_hash_algo);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_DIGESTS;
        spdm_response->header.param2 = 0;
        libspdm_set_mem(m_libspdm_local_certificate_chain,
                        sizeof(m_libspdm_local_certificate_chain),
                        (uint8_t)(0xFF));

        digest = (void *)(spdm_response + 1);
        libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                         sizeof(m_libspdm_local_certificate_chain), &digest[0]);
        spdm_response->header.param2 |= (1 << 0);

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response, response_size,
                                              response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x4: {
        spdm_error_response_t *spdm_response;
        size_t spdm_response_size;
        size_t transport_header_size;

        spdm_response_size = sizeof(spdm_error_response_t);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.request_response_code = SPDM_ERROR;
        spdm_response->header.param1 = SPDM_ERROR_CODE_INVALID_REQUEST;
        spdm_response->header.param2 = 0;

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response,
                                              response_size, response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x5: {
        spdm_error_response_t *spdm_response;
        size_t spdm_response_size;
        size_t transport_header_size;

        spdm_response_size = sizeof(spdm_error_response_t);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.request_response_code = SPDM_ERROR;
        spdm_response->header.param1 = SPDM_ERROR_CODE_BUSY;
        spdm_response->header.param2 = 0;

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response,
                                              response_size, response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x6: {
        static size_t sub_index1 = 0;
        if (sub_index1 == 0) {
            spdm_error_response_t *spdm_response;
            size_t spdm_response_size;
            size_t transport_header_size;

            spdm_response_size = sizeof(spdm_error_response_t);
            transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
            spdm_response = (void *)((uint8_t *)*response + transport_header_size);

            spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
            spdm_response->header.request_response_code = SPDM_ERROR;
            spdm_response->header.param1 = SPDM_ERROR_CODE_BUSY;
            spdm_response->header.param2 = 0;

            libspdm_transport_test_encode_message(
                spdm_context, NULL, false, false,
                spdm_response_size, spdm_response,
                response_size, response);
        } else if (sub_index1 == 1) {
            spdm_digest_response_t *spdm_response;
            uint8_t *digest;
            size_t spdm_response_size;
            size_t transport_header_size;

            ((libspdm_context_t *)spdm_context)
            ->connection_info.algorithm.base_hash_algo =
                m_libspdm_use_hash_algo;
            spdm_response_size = sizeof(spdm_digest_response_t) +
                                 libspdm_get_hash_size(m_libspdm_use_hash_algo);
            transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
            spdm_response = (void *)((uint8_t *)*response + transport_header_size);

            spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
            spdm_response->header.param1 = 0;
            spdm_response->header.request_response_code = SPDM_DIGESTS;
            spdm_response->header.param2 = 0;
            libspdm_set_mem(m_libspdm_local_certificate_chain,
                            sizeof(m_libspdm_local_certificate_chain), (uint8_t)(0xFF));

            digest = (void *)(spdm_response + 1);
            libspdm_hash_all(m_libspdm_use_hash_algo,
                             m_libspdm_local_certificate_chain,
                             sizeof(m_libspdm_local_certificate_chain), &digest[0]);
            spdm_response->header.param2 |= (1 << 0);

            libspdm_transport_test_encode_message(
                spdm_context, NULL, false, false, spdm_response_size,
                spdm_response, response_size, response);
        }
        sub_index1++;
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x7: {
        spdm_error_response_t *spdm_response;
        size_t spdm_response_size;
        size_t transport_header_size;

        spdm_response_size = sizeof(spdm_error_response_t);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.request_response_code = SPDM_ERROR;
        spdm_response->header.param1 = SPDM_ERROR_CODE_REQUEST_RESYNCH;
        spdm_response->header.param2 = 0;

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response,
                                              response_size, response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x8: {
        spdm_error_response_data_response_not_ready_t *spdm_response;
        size_t spdm_response_size;
        size_t transport_header_size;

        spdm_response_size = sizeof(spdm_error_response_data_response_not_ready_t);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.request_response_code = SPDM_ERROR;
        spdm_response->header.param1 = SPDM_ERROR_CODE_RESPONSE_NOT_READY;
        spdm_response->header.param2 = 0;
        spdm_response->extend_error_data.rd_exponent = 1;
        spdm_response->extend_error_data.rd_tm = 1;
        spdm_response->extend_error_data.request_code = SPDM_GET_DIGESTS;
        spdm_response->extend_error_data.token = 0;

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response,
                                              response_size, response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x9: {
        static size_t sub_index2 = 0;
        if (sub_index2 == 0) {
            spdm_error_response_data_response_not_ready_t
            *spdm_response;
            size_t spdm_response_size;
            size_t transport_header_size;

            spdm_response_size = sizeof(spdm_error_response_data_response_not_ready_t);
            transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
            spdm_response = (void *)((uint8_t *)*response + transport_header_size);

            spdm_response->header.spdm_version =
                SPDM_MESSAGE_VERSION_10;
            spdm_response->header.request_response_code = SPDM_ERROR;
            spdm_response->header.param1 =
                SPDM_ERROR_CODE_RESPONSE_NOT_READY;
            spdm_response->header.param2 = 0;
            spdm_response->extend_error_data.rd_exponent = 1;
            spdm_response->extend_error_data.rd_tm = 1;
            spdm_response->extend_error_data.request_code =
                SPDM_GET_DIGESTS;
            spdm_response->extend_error_data.token = 1;

            libspdm_transport_test_encode_message(
                spdm_context, NULL, false, false,
                spdm_response_size, spdm_response,
                response_size, response);
        } else if (sub_index2 == 1) {
            spdm_digest_response_t *spdm_response;
            uint8_t *digest;
            size_t spdm_response_size;
            size_t transport_header_size;

            ((libspdm_context_t *)spdm_context)
            ->connection_info.algorithm.base_hash_algo =
                m_libspdm_use_hash_algo;
            spdm_response_size = sizeof(spdm_digest_response_t) +
                                 libspdm_get_hash_size(m_libspdm_use_hash_algo);
            transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
            spdm_response = (void *)((uint8_t *)*response + transport_header_size);

            spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
            spdm_response->header.param1 = 0;
            spdm_response->header.request_response_code = SPDM_DIGESTS;
            spdm_response->header.param2 = 0;
            libspdm_set_mem(m_libspdm_local_certificate_chain,
                            sizeof(m_libspdm_local_certificate_chain), (uint8_t)(0xFF));

            digest = (void *)(spdm_response + 1);
            libspdm_hash_all(m_libspdm_use_hash_algo,
                             m_libspdm_local_certificate_chain,
                             sizeof(m_libspdm_local_certificate_chain), &digest[0]);
            spdm_response->header.param2 |= (1 << 0);

            libspdm_transport_test_encode_message(
                spdm_context, NULL, false, false, spdm_response_size,
                spdm_response, response_size, response);
        }
        sub_index2++;
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0xA:
        return LIBSPDM_STATUS_SUCCESS;

    case 0xB:
        return LIBSPDM_STATUS_RECEIVE_FAIL;

    case 0xC: {
        spdm_digest_response_t *spdm_response;
        size_t spdm_response_size;
        size_t transport_header_size;

        ((libspdm_context_t *)spdm_context)
        ->connection_info.algorithm.base_hash_algo =
            m_libspdm_use_hash_algo;
        spdm_response_size = 2;
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_DIGESTS;
        spdm_response->header.param2 = 0;

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response, response_size,
                                              response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0xD: {
        spdm_digest_response_t *spdm_response;
        uint8_t *digest;
        size_t spdm_response_size;
        size_t transport_header_size;

        ((libspdm_context_t *)spdm_context)
        ->connection_info.algorithm.base_hash_algo =
            m_libspdm_use_hash_algo;
        spdm_response_size = sizeof(spdm_digest_response_t) +
                             libspdm_get_hash_size(m_libspdm_use_hash_algo);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_CERTIFICATE;
        spdm_response->header.param2 = 0;
        libspdm_set_mem(m_libspdm_local_certificate_chain,
                        sizeof(m_libspdm_local_certificate_chain),
                        (uint8_t)(0xFF));

        digest = (void *)(spdm_response + 1);
        libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                         sizeof(m_libspdm_local_certificate_chain), &digest[0]);
        spdm_response->header.param2 |= (1 << 0);

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response, response_size,
                                              response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0xE: {
        spdm_digest_response_t *spdm_response;
        size_t spdm_response_size;
        size_t transport_header_size;

        spdm_response_size = sizeof(spdm_digest_response_t);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_DIGESTS;
        spdm_response->header.param2 = 0;

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response,
                                              response_size, response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0xF:
        return LIBSPDM_STATUS_SUCCESS;

    case 0x10: {
        spdm_digest_response_t *spdm_response;
        uint8_t *digest;
        size_t spdm_response_size;
        size_t transport_header_size;

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
        ((libspdm_context_t *)spdm_context)->transcript.message_b.buffer_size =
            ((libspdm_context_t *)spdm_context)->transcript.message_b.max_buffer_size -
            (sizeof(spdm_digest_response_t));
#endif

        ((libspdm_context_t *)spdm_context)
        ->connection_info.algorithm.base_hash_algo =
            m_libspdm_use_hash_algo;
        spdm_response_size = sizeof(spdm_digest_response_t) +
                             libspdm_get_hash_size(m_libspdm_use_hash_algo);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_DIGESTS;
        spdm_response->header.param2 = 0;
        libspdm_set_mem(m_libspdm_local_certificate_chain,
                        sizeof(m_libspdm_local_certificate_chain),
                        (uint8_t)(0xFF));

        digest = (void *)(spdm_response + 1);
        libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                         sizeof(m_libspdm_local_certificate_chain), &digest[0]);
        spdm_response->header.param2 |= (1 << 0);

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response, response_size,
                                              response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x11: {
        spdm_digest_response_t *spdm_response;
        uint8_t *digest;
        size_t spdm_response_size;
        size_t transport_header_size;

        ((libspdm_context_t *)spdm_context)
        ->connection_info.algorithm.base_hash_algo =
            m_libspdm_use_hash_algo;
        spdm_response_size = sizeof(spdm_digest_response_t) +
                             libspdm_get_hash_size(m_libspdm_use_hash_algo);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_DIGESTS;
        spdm_response->header.param2 = 0;
        libspdm_set_mem(m_libspdm_local_certificate_chain,
                        sizeof(m_libspdm_local_certificate_chain),
                        (uint8_t)(0xFF));

        digest = (void *)(spdm_response + 1);
        libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                         sizeof(m_libspdm_local_certificate_chain), &digest[0]);
        digest[libspdm_get_hash_size(m_libspdm_use_hash_algo) - 1] = 0;
        spdm_response->header.param2 |= (1 << 0);

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response, response_size,
                                              response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x12: {
        spdm_digest_response_t *spdm_response;
        uint8_t *digest;
        size_t digest_count;
        size_t spdm_response_size;
        size_t transport_header_size;
        size_t index;

        ((libspdm_context_t *)spdm_context)
        ->connection_info.algorithm.base_hash_algo =
            m_libspdm_use_hash_algo;
        digest_count = 4;
        spdm_response_size = sizeof(spdm_digest_response_t) +
                             libspdm_get_hash_size(m_libspdm_use_hash_algo);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_DIGESTS;
        spdm_response->header.param2 = 0;
        libspdm_set_mem(m_libspdm_local_certificate_chain,
                        sizeof(m_libspdm_local_certificate_chain),
                        (uint8_t)(0xFF));

        digest = (void *)(spdm_response + 1);

        libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                         sizeof(m_libspdm_local_certificate_chain), &digest[0]);
        for (index = 0; index < digest_count; index++) {
            spdm_response->header.param2 |= (1 << index);
        }

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response, response_size,
                                              response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x13: {
        spdm_digest_response_t *spdm_response;
        uint8_t *digest;
        size_t digest_count;
        size_t spdm_response_size;
        size_t transport_header_size;
        size_t index;

        ((libspdm_context_t *)spdm_context)
        ->connection_info.algorithm.base_hash_algo =
            m_libspdm_use_hash_algo;
        digest_count = 4;
        spdm_response_size =
            sizeof(spdm_digest_response_t) +
            digest_count * libspdm_get_hash_size(m_libspdm_use_hash_algo);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_DIGESTS;
        spdm_response->header.param2 = 0;
        libspdm_set_mem(m_libspdm_local_certificate_chain,
                        sizeof(m_libspdm_local_certificate_chain),
                        (uint8_t)(0xFF));

        digest = (void *)(spdm_response + 1);

        for (index = 0; index < digest_count; index++) {
            libspdm_hash_all(
                m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                sizeof(m_libspdm_local_certificate_chain),
                &digest[index *
                        libspdm_get_hash_size(m_libspdm_use_hash_algo)]);
            spdm_response->header.param2 |= (1 << index);
            if (index == 0)
                continue;
            digest[(index + 1) * libspdm_get_hash_size(m_libspdm_use_hash_algo) -
                   1] = 0;
        }

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response, response_size,
                                              response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x14: {
        spdm_digest_response_t *spdm_response;
        size_t spdm_response_size;
        size_t transport_header_size;

        ((libspdm_context_t *)spdm_context)
        ->connection_info.algorithm.base_hash_algo =
            m_libspdm_use_hash_algo;
        spdm_response_size = 5;
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_DIGESTS;
        spdm_response->header.param2 = 0;

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response, response_size,
                                              response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x15: {
        spdm_digest_response_t *spdm_response;
        uint8_t *digest;
        size_t spdm_response_size;
        size_t transport_header_size;

        ((libspdm_context_t *)spdm_context)
        ->connection_info.algorithm.base_hash_algo =
            m_libspdm_use_hash_algo;
        spdm_response_size = sizeof(spdm_message_header_t) +
                             LIBSPDM_MAX_HASH_SIZE * SPDM_MAX_SLOT_COUNT + 1;
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_DIGESTS;
        spdm_response->header.param2 = 0;
        libspdm_set_mem(m_libspdm_local_certificate_chain,
                        sizeof(m_libspdm_local_certificate_chain),
                        (uint8_t)(0xFF));

        digest = (void *)(spdm_response + 1);
        libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                         sizeof(m_libspdm_local_certificate_chain), &digest[0]);
        spdm_response->header.param2 |= (1 << 0);

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response, response_size,
                                              response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x16:
    {
        static uint16_t error_code = LIBSPDM_ERROR_CODE_RESERVED_00;

        spdm_error_response_t *spdm_response;
        size_t spdm_response_size;
        size_t transport_header_size;

        spdm_response_size = sizeof(spdm_error_response_t);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        if(error_code <= 0xff) {
            libspdm_zero_mem (spdm_response, spdm_response_size);
            spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_11;
            spdm_response->header.request_response_code = SPDM_ERROR;
            spdm_response->header.param1 = (uint8_t) error_code;
            spdm_response->header.param2 = 0;

            libspdm_transport_test_encode_message (spdm_context, NULL, false, false,
                                                   spdm_response_size, spdm_response,
                                                   response_size, response);
        }

        error_code++;
        if(error_code == SPDM_ERROR_CODE_BUSY) { /*busy is treated in cases 5 and 6*/
            error_code = SPDM_ERROR_CODE_UNEXPECTED_REQUEST;
        }
        if(error_code == LIBSPDM_ERROR_CODE_RESERVED_0D) { /*skip some reserved error codes (0d to 3e)*/
            error_code = LIBSPDM_ERROR_CODE_RESERVED_3F;
        }
        if(error_code == SPDM_ERROR_CODE_RESPONSE_NOT_READY) { /*skip response not ready, request resync, and some reserved codes (44 to fc)*/
            error_code = LIBSPDM_ERROR_CODE_RESERVED_FD;
        }
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x17: {
        spdm_digest_response_t *spdm_response;
        uint8_t *digest;
        size_t spdm_response_size;
        size_t transport_header_size;
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
        size_t arbitrary_size;
#endif

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
        /*filling B with arbitrary data*/
        arbitrary_size = 8;
        libspdm_set_mem(((libspdm_context_t *)spdm_context)->transcript.message_b.buffer,
                        arbitrary_size, (uint8_t) 0xEE);
        ((libspdm_context_t *)spdm_context)->transcript.message_b.buffer_size =
            arbitrary_size;
#endif

        ((libspdm_context_t *)spdm_context)->connection_info.algorithm.base_hash_algo =
            m_libspdm_use_hash_algo;
        spdm_response_size = sizeof(spdm_digest_response_t) +
                             libspdm_get_hash_size(m_libspdm_use_hash_algo);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_DIGESTS;
        spdm_response->header.param2 = 0;
        libspdm_set_mem(m_libspdm_local_certificate_chain,
                        sizeof(m_libspdm_local_certificate_chain),
                        (uint8_t)(0xFF));

        digest = (void *)(spdm_response + 1);
        libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                         sizeof(m_libspdm_local_certificate_chain), &digest[0]);
        spdm_response->header.param2 |= (0x01 << 0);

        libspdm_copy_mem(&m_libspdm_local_buffer[m_libspdm_local_buffer_size],
                         sizeof(m_libspdm_local_buffer) - m_libspdm_local_buffer_size,
                         spdm_response, spdm_response_size);
        m_libspdm_local_buffer_size += spdm_response_size;

        libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                              false, spdm_response_size,
                                              spdm_response, response_size,
                                              response);
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x18: {
        if (m_get_digest) {
            spdm_digest_response_t *spdm_response;
            uint8_t *digest;
            size_t spdm_response_size;
            size_t transport_header_size;

            ((libspdm_context_t *)spdm_context)
            ->connection_info.algorithm.base_hash_algo =
                m_libspdm_use_hash_algo;
            spdm_response_size = sizeof(spdm_digest_response_t) +
                                 libspdm_get_hash_size(m_libspdm_use_hash_algo) *
                                 SPDM_MAX_SLOT_COUNT;
            transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
            spdm_response = (void *)((uint8_t *)*response + transport_header_size);

            spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
            spdm_response->header.param1 = 0;
            spdm_response->header.request_response_code = SPDM_DIGESTS;
            spdm_response->header.param2 = 0;

            if (m_libspdm_local_certificate_chain_test_cert == NULL) {
                libspdm_read_responder_public_certificate_chain(
                    m_libspdm_use_hash_algo, m_libspdm_use_asym_algo,
                    &m_libspdm_local_certificate_chain_test_cert,
                    &m_libspdm_local_certificate_chain_size, NULL, NULL);
            }
            if (m_libspdm_local_certificate_chain_test_cert == NULL) {
                return LIBSPDM_STATUS_RECEIVE_FAIL;
            }
            digest = (void *)(spdm_response + 1);
            libspdm_zero_mem(digest,
                             libspdm_get_hash_size(m_libspdm_use_hash_algo) * SPDM_MAX_SLOT_COUNT);
            libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain_test_cert,
                             m_libspdm_local_certificate_chain_size, &digest[0]);
            spdm_response->header.param2 |= (0x01 << 0);

            libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                                  false, spdm_response_size,
                                                  spdm_response, response_size,
                                                  response);
        } else {
            spdm_certificate_response_t *spdm_response;
            size_t spdm_response_size;
            size_t transport_header_size;
            uint16_t portion_length;
            uint16_t remainder_length;
            size_t count;
            static size_t calling_index = 0;

            if (m_libspdm_local_certificate_chain_test_cert == NULL) {
                libspdm_read_responder_public_certificate_chain(
                    m_libspdm_use_hash_algo, m_libspdm_use_asym_algo,
                    &m_libspdm_local_certificate_chain_test_cert,
                    &m_libspdm_local_certificate_chain_size, NULL, NULL);
            }
            if (m_libspdm_local_certificate_chain_test_cert == NULL) {
                return LIBSPDM_STATUS_RECEIVE_FAIL;
            }
            count = (m_libspdm_local_certificate_chain_size +
                     LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN - 1) /
                    LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN;
            if (calling_index != count - 1) {
                portion_length = LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN;
                remainder_length =
                    (uint16_t)(m_libspdm_local_certificate_chain_size -
                               LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN *
                               (calling_index + 1));
            } else {
                portion_length = (uint16_t)(
                    m_libspdm_local_certificate_chain_size -
                    LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN * (count - 1));
                remainder_length = 0;
            }

            spdm_response_size =
                sizeof(spdm_certificate_response_t) + portion_length;
            transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
            spdm_response = (void *)((uint8_t *)*response + transport_header_size);

            spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
            spdm_response->header.request_response_code = SPDM_CERTIFICATE;
            spdm_response->header.param1 = 0;
            spdm_response->header.param2 = 0;
            spdm_response->portion_length = portion_length;
            spdm_response->remainder_length = remainder_length;
            libspdm_copy_mem(spdm_response + 1,
                             (size_t)(*response) + *response_size - (size_t)(spdm_response + 1),
                             (uint8_t *)m_libspdm_local_certificate_chain_test_cert +
                             LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN * calling_index,
                             portion_length);

            libspdm_transport_test_encode_message(spdm_context, NULL, false,
                                                  false, spdm_response_size,
                                                  spdm_response, response_size,
                                                  response);

            calling_index++;
            if (calling_index == count) {
                calling_index = 0;
                free(m_libspdm_local_certificate_chain_test_cert);
                m_libspdm_local_certificate_chain_test_cert = NULL;
                m_libspdm_local_certificate_chain_size = 0;
            }
        }
    }
        return LIBSPDM_STATUS_SUCCESS;

    case 0x19: {
        spdm_digest_response_t *spdm_response;
        uint8_t *digest;
        size_t spdm_response_size;
        size_t transport_header_size;
        uint32_t session_id;
        libspdm_session_info_t *session_info;
        uint8_t *scratch_buffer;
        size_t scratch_buffer_size;

        session_id = 0xFFFFFFFF;

        ((libspdm_context_t *)spdm_context)
        ->connection_info.algorithm.base_hash_algo =
            m_libspdm_use_hash_algo;
        spdm_response_size = sizeof(spdm_digest_response_t) +
                             libspdm_get_hash_size(m_libspdm_use_hash_algo);
        transport_header_size = libspdm_transport_test_get_header_size(spdm_context);
        spdm_response = (void *)((uint8_t *)*response + transport_header_size);

        spdm_response->header.spdm_version = SPDM_MESSAGE_VERSION_10;
        spdm_response->header.param1 = 0;
        spdm_response->header.request_response_code = SPDM_DIGESTS;
        spdm_response->header.param2 = 0;

        /* For secure message, message is in sender buffer, we need copy it to scratch buffer.
         * transport_message is always in sender buffer. */
        libspdm_get_scratch_buffer (spdm_context, (void **)&scratch_buffer, &scratch_buffer_size);

        libspdm_set_mem(m_libspdm_local_certificate_chain,
                        sizeof(m_libspdm_local_certificate_chain),
                        (uint8_t)(0xFF));

        digest = (void *)(spdm_response + 1);
        /* send certchain digest of slot 7 */
        libspdm_zero_mem (digest,
                          libspdm_get_hash_size(m_libspdm_use_hash_algo) * SPDM_MAX_SLOT_COUNT);
        libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                         sizeof(m_libspdm_local_certificate_chain), &digest[0]);
        spdm_response->header.param2 |= (0x80 << 0);

        libspdm_copy_mem (scratch_buffer + transport_header_size,
                          scratch_buffer_size - transport_header_size,
                          spdm_response, spdm_response_size);
        spdm_response = (void *)(scratch_buffer + transport_header_size);
        libspdm_transport_test_encode_message(spdm_context, &session_id, false,
                                              false, spdm_response_size,
                                              spdm_response, response_size,
                                              response);

        session_info = libspdm_get_session_info_via_session_id(
            spdm_context, session_id);
        if (session_info == NULL) {
            return LIBSPDM_STATUS_RECEIVE_FAIL;
        }
        /* WALKAROUND: If just use single context to encode message and then decode message */
        ((libspdm_secured_message_context_t
          *)(session_info->secured_message_context))
        ->application_secret.response_data_sequence_number--;

    }
        return LIBSPDM_STATUS_SUCCESS;

    default:
        return LIBSPDM_STATUS_RECEIVE_FAIL;
    }
}

/**
 * Test 1:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case1(void **state)
{
}

/**
 * Test 2: a request message is successfully sent and a response message is successfully received
 * Expected Behavior: requester returns the status LIBSPDM_STATUS_SUCCESS and a DIGESTS message is received
 **/
static void libspdm_test_requester_get_digests_case2(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    uint8_t slot_mask;
    uint8_t total_digest_buffer[LIBSPDM_MAX_HASH_SIZE * SPDM_MAX_SLOT_COUNT];
    uint8_t my_total_digest_buffer[LIBSPDM_MAX_HASH_SIZE * SPDM_MAX_SLOT_COUNT];
    uint8_t *digest;
    size_t data_return_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x2;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_10 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    libspdm_set_mem(m_libspdm_local_certificate_chain,
                    sizeof(m_libspdm_local_certificate_chain),
                    (uint8_t)(0xFF));
    libspdm_reset_message_b(spdm_context);

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    spdm_context->transcript.message_m.buffer_size =
        spdm_context->transcript.message_m.max_buffer_size;
#endif
    libspdm_zero_mem(total_digest_buffer, sizeof(total_digest_buffer));
    status = libspdm_get_digest(spdm_context, NULL, &slot_mask, &total_digest_buffer);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);

    assert_int_equal(slot_mask, 0x01);
    libspdm_zero_mem(my_total_digest_buffer, sizeof(my_total_digest_buffer));
    digest = my_total_digest_buffer;
    libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                     sizeof(m_libspdm_local_certificate_chain), digest);
    assert_memory_equal (total_digest_buffer, my_total_digest_buffer,
                         sizeof(my_total_digest_buffer));

    data_return_size = sizeof(uint8_t);
    status = libspdm_get_data(spdm_context, LIBSPDM_DATA_PEER_SLOT_MASK,
                              NULL, &slot_mask, &data_return_size);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(data_return_size, sizeof(uint8_t));
    assert_int_equal(slot_mask, 0x01);

    data_return_size = sizeof(total_digest_buffer);
    status = libspdm_get_data(spdm_context, LIBSPDM_DATA_PEER_TOTAL_DIGEST_BUFFER,
                              NULL, total_digest_buffer, &data_return_size);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(data_return_size, libspdm_get_hash_size(m_libspdm_use_hash_algo));
    assert_memory_equal (total_digest_buffer, my_total_digest_buffer,
                         sizeof(my_total_digest_buffer));

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(
        spdm_context->transcript.message_b.buffer_size,
        sizeof(spdm_get_digest_request_t) +
        sizeof(spdm_digest_response_t) +
        libspdm_get_hash_size(spdm_context->connection_info.algorithm.base_hash_algo));
    assert_int_equal(spdm_context->transcript.message_m.buffer_size, 0);
#endif
}

/**
 * Test 3:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case3(void **state)
{
}

/**
 * Test 4:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case4(void **state)
{
}

/**
 * Test 5:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case5(void **state)
{
}

/**
 * Test 6:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case6(void **state)
{
}

/**
 * Test 7:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case7(void **state)
{
}

/**
 * Test 8:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case8(void **state)
{
}

/**
 * Test 9:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case9(void **state)
{
}

/**
 * Test 10:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case10(void **state)
{
}

/**
 * Test 11:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case11(void **state)
{
}

/**
 * Test 12:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case12(void **state)
{
}

/**
 * Test 13:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case13(void **state)
{
}

/**
 * Test 14:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case14(void **state)
{
}

/**
 * Test 15:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case15(void **state)
{
}

/**
 * Test 16:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case16(void **state)
{
}

/**
 * Test 17:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case17(void **state)
{
}

/**
 * Test 18:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case18(void **state)
{
}

/**
 * Test 19:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case19(void **state)
{
}

/**
 * Test 20:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case20(void **state)
{
}

/**
 * Test 21:
 * Expected Behavior:
 **/
static void libspdm_test_requester_get_digests_case21(void **state)
{
}

/**
 * Test 22:
 * Expected behavior:.
 **/
static void libspdm_test_requester_get_digests_case22(void **state)
{
}

/**
 * Test 23: a request message is successfully sent and a response message is successfully received.
 * Buffer B already has arbitrary data.
 * Expected Behavior: requester returns the status RETURN_SUCCESS and a DIGESTS message is
 * received, buffer B appends the exchanged GET_DIGESTS and DIGESTS messages.
 **/
static void libspdm_test_requester_get_digests_case23(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    uint8_t slot_mask;
    uint8_t total_digest_buffer[LIBSPDM_MAX_HASH_SIZE * SPDM_MAX_SLOT_COUNT];
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    size_t arbitrary_size;
#endif

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x17;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_10 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    libspdm_set_mem(m_libspdm_local_certificate_chain,
                    sizeof(m_libspdm_local_certificate_chain),
                    (uint8_t)(0xFF));
    libspdm_reset_message_b(spdm_context);

    libspdm_zero_mem(total_digest_buffer, sizeof(total_digest_buffer));
    status = libspdm_get_digest(spdm_context, NULL, &slot_mask, &total_digest_buffer);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    arbitrary_size = 8;
    assert_int_equal(spdm_context->transcript.message_b.buffer_size,
                     arbitrary_size + m_libspdm_local_buffer_size);
    LIBSPDM_DEBUG((LIBSPDM_DEBUG_INFO, "m_libspdm_local_buffer (0x%x):\n",
                   m_libspdm_local_buffer_size));
    libspdm_dump_hex(m_libspdm_local_buffer, m_libspdm_local_buffer_size);
    assert_memory_equal(spdm_context->transcript.message_b.buffer + arbitrary_size,
                        m_libspdm_local_buffer, m_libspdm_local_buffer_size);
#endif
}

/**
 * Test 24: Test case for GetDigest, GetCert and GetDigest
 * Expected Behavior: requester returns the status LIBSPDM_STATUS_SUCCESS and a second GetDigest can be sent.
 **/
static void libspdm_test_requester_get_digests_case24(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    uint8_t slot_mask;
    uint8_t total_digest_buffer[LIBSPDM_MAX_HASH_SIZE * SPDM_MAX_SLOT_COUNT];
    uint8_t my_total_digest_buffer[LIBSPDM_MAX_HASH_SIZE * SPDM_MAX_SLOT_COUNT];
    uint8_t *digest;
    size_t data_return_size;
    void *data;
    size_t data_size;
    void *hash;
    size_t hash_size;
    const uint8_t *root_cert;
    size_t root_cert_size;
    size_t cert_chain_size;
    uint8_t cert_chain[LIBSPDM_MAX_CERT_CHAIN_SIZE];

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x18;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_10 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo =
        m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.req_base_asym_alg =
        m_libspdm_use_req_asym_algo;
    spdm_context->local_context.is_requester = true;

    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data,
                                                    &data_size, &hash, &hash_size);
    libspdm_x509_get_cert_from_cert_chain(
        (uint8_t *)data + sizeof(spdm_cert_chain_t) + hash_size,
        data_size - sizeof(spdm_cert_chain_t) - hash_size, 0,
        &root_cert, &root_cert_size);
    LIBSPDM_DEBUG((LIBSPDM_DEBUG_INFO, "root cert data :\n"));
    libspdm_dump_hex(root_cert, root_cert_size);
    spdm_context->local_context.peer_root_cert_provision_size[0] =
        root_cert_size;
    spdm_context->local_context.peer_root_cert_provision[0] = root_cert;

    m_get_digest = true;
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    spdm_context->transcript.message_m.buffer_size =
        spdm_context->transcript.message_m.max_buffer_size;
#endif
    libspdm_zero_mem(total_digest_buffer, sizeof(total_digest_buffer));
    /* first GetDigest */
    status = libspdm_get_digest(spdm_context, NULL, &slot_mask, &total_digest_buffer);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(slot_mask, 0x01);
    libspdm_zero_mem(my_total_digest_buffer, sizeof(my_total_digest_buffer));
    digest = my_total_digest_buffer;
    if (m_libspdm_local_certificate_chain_test_cert == NULL) {
        libspdm_read_responder_public_certificate_chain(
            m_libspdm_use_hash_algo, m_libspdm_use_asym_algo,
            &m_libspdm_local_certificate_chain_test_cert,
            &m_libspdm_local_certificate_chain_size, NULL, NULL);
    }
    libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain_test_cert,
                     m_libspdm_local_certificate_chain_size, digest);
    assert_memory_equal(total_digest_buffer, my_total_digest_buffer,
                        sizeof(my_total_digest_buffer));
    data_return_size = sizeof(uint8_t);
    status = libspdm_get_data(spdm_context, LIBSPDM_DATA_PEER_SLOT_MASK,
                              NULL, &slot_mask, &data_return_size);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(data_return_size, sizeof(uint8_t));
    assert_int_equal(slot_mask, 0x01);

    data_return_size = sizeof(total_digest_buffer);
    status = libspdm_get_data(spdm_context, LIBSPDM_DATA_PEER_TOTAL_DIGEST_BUFFER,
                              NULL, total_digest_buffer, &data_return_size);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(data_return_size, libspdm_get_hash_size(m_libspdm_use_hash_algo));
    assert_memory_equal (total_digest_buffer, my_total_digest_buffer,
                         sizeof(my_total_digest_buffer));
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(
        spdm_context->transcript.message_b.buffer_size,
        sizeof(spdm_get_digest_request_t) +
        sizeof(spdm_digest_response_t) +
        libspdm_get_hash_size(spdm_context->connection_info.algorithm.base_hash_algo));
    assert_int_equal(spdm_context->transcript.message_m.buffer_size, 0);
#endif

    m_get_digest = false;
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    spdm_context->transcript.message_m.buffer_size =
        spdm_context->transcript.message_m.max_buffer_size;
#endif
    cert_chain_size = sizeof(cert_chain);
    libspdm_zero_mem(cert_chain, sizeof(cert_chain));
    status = libspdm_get_certificate(spdm_context, NULL, 0, &cert_chain_size,
                                     cert_chain);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(spdm_context->transcript.message_m.buffer_size, 0);
#endif
    free(data);

    m_get_digest = true;
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    spdm_context->transcript.message_m.buffer_size =
        spdm_context->transcript.message_m.max_buffer_size;
#endif
    libspdm_zero_mem(total_digest_buffer, sizeof(total_digest_buffer));
    /* second GetDigest */
    status = libspdm_get_digest(spdm_context, NULL, &slot_mask, &total_digest_buffer);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(slot_mask, 0x01);
    libspdm_zero_mem(my_total_digest_buffer, sizeof(my_total_digest_buffer));
    digest = my_total_digest_buffer;
    libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain_test_cert,
                     m_libspdm_local_certificate_chain_size, digest);
    assert_memory_equal (total_digest_buffer, my_total_digest_buffer,
                         sizeof(my_total_digest_buffer));
    data_return_size = sizeof(uint8_t);
    status = libspdm_get_data(spdm_context, LIBSPDM_DATA_PEER_SLOT_MASK,
                              NULL, &slot_mask, &data_return_size);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(data_return_size, sizeof(uint8_t));
    assert_int_equal(slot_mask, 0x01);
    data_return_size = sizeof(total_digest_buffer);
    status = libspdm_get_data(spdm_context, LIBSPDM_DATA_PEER_TOTAL_DIGEST_BUFFER,
                              NULL, total_digest_buffer, &data_return_size);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(data_return_size, libspdm_get_hash_size(m_libspdm_use_hash_algo));
    assert_memory_equal (total_digest_buffer, my_total_digest_buffer,
                         sizeof(my_total_digest_buffer));
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(
        spdm_context->transcript.message_b.buffer_size,
        sizeof(spdm_get_digest_request_t) +
        sizeof(spdm_digest_response_t) +
        libspdm_get_hash_size(spdm_context->connection_info.algorithm.base_hash_algo));
    assert_int_equal(spdm_context->transcript.message_m.buffer_size, 0);
    spdm_context->connection_info.peer_used_cert_chain[0].buffer_size = 0;
#else
    spdm_context->connection_info.peer_used_cert_chain[0].buffer_hash_size = 0;
#endif
}

/**
 * Test 25: a request message is successfully sent and a response message is successfully received
 * in a session.
 * Expected Behavior: requester returns the status LIBSPDM_STATUS_SUCCESS and a DIGESTS message is received
 **/
static void libspdm_test_requester_get_digests_case25(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    uint8_t slot_mask;
    uint8_t total_digest_buffer[LIBSPDM_MAX_HASH_SIZE * SPDM_MAX_SLOT_COUNT];
    uint8_t my_total_digest_buffer[LIBSPDM_MAX_HASH_SIZE * SPDM_MAX_SLOT_COUNT];
    uint8_t *digest;
    size_t data_return_size;
    uint32_t session_id;
    libspdm_session_info_t *session_info;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x19;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_10 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCRYPT_CAP;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MAC_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCRYPT_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_MAC_CAP;
    spdm_context->connection_info.algorithm.dhe_named_group =
        m_libspdm_use_dhe_algo;
    spdm_context->connection_info.algorithm.aead_cipher_suite =
        m_libspdm_use_aead_algo;

    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;

    session_id = 0xFFFFFFFF;
    session_info = &spdm_context->session_info[0];
    libspdm_session_info_init(spdm_context, session_info, session_id, true);
    libspdm_secured_message_set_session_state(session_info->secured_message_context,
                                              LIBSPDM_SESSION_STATE_ESTABLISHED);

    libspdm_set_mem(m_libspdm_local_certificate_chain,
                    sizeof(m_libspdm_local_certificate_chain),
                    (uint8_t)(0xFF));
    libspdm_reset_message_b(spdm_context);

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    session_info->session_transcript.message_m.buffer_size =
        session_info->session_transcript.message_m.max_buffer_size;
#endif
    libspdm_zero_mem(total_digest_buffer, sizeof(total_digest_buffer));
    status = libspdm_get_digest(spdm_context, &session_id, &slot_mask, &total_digest_buffer);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);

    assert_int_equal(slot_mask, 0x80);
    libspdm_zero_mem(my_total_digest_buffer, sizeof(my_total_digest_buffer));
    digest = my_total_digest_buffer;
    libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                     sizeof(m_libspdm_local_certificate_chain), digest);
    assert_memory_equal (total_digest_buffer, my_total_digest_buffer,
                         sizeof(my_total_digest_buffer));

    data_return_size = sizeof(uint8_t);
    status = libspdm_get_data(spdm_context, LIBSPDM_DATA_PEER_SLOT_MASK,
                              NULL, &slot_mask, &data_return_size);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(data_return_size, sizeof(uint8_t));
    assert_int_equal(slot_mask, 0x80);

    data_return_size = sizeof(total_digest_buffer);
    status = libspdm_get_data(spdm_context, LIBSPDM_DATA_PEER_TOTAL_DIGEST_BUFFER,
                              NULL, total_digest_buffer, &data_return_size);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(data_return_size, libspdm_get_hash_size(m_libspdm_use_hash_algo));
    assert_memory_equal (total_digest_buffer, my_total_digest_buffer,
                         sizeof(my_total_digest_buffer));

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(session_info->session_transcript.message_m.buffer_size, 0);
#endif
}

static libspdm_test_context_t m_libspdm_requester_get_digests_test_context = {
    LIBSPDM_TEST_CONTEXT_VERSION,
    true,
    libspdm_requester_get_digests_test_send_message,
    libspdm_requester_get_digests_test_receive_message,
};

int libspdm_requester_get_digests_test_main(void)
{
    const struct CMUnitTest spdm_requester_get_digests_tests[] = {
        cmocka_unit_test(libspdm_test_requester_get_digests_case1),
        cmocka_unit_test(libspdm_test_requester_get_digests_case2),
        cmocka_unit_test(libspdm_test_requester_get_digests_case3),
        cmocka_unit_test(libspdm_test_requester_get_digests_case4),
        cmocka_unit_test(libspdm_test_requester_get_digests_case5),
        cmocka_unit_test(libspdm_test_requester_get_digests_case6),
        cmocka_unit_test(libspdm_test_requester_get_digests_case7),
        cmocka_unit_test(libspdm_test_requester_get_digests_case8),
        cmocka_unit_test(libspdm_test_requester_get_digests_case9),
        cmocka_unit_test(libspdm_test_requester_get_digests_case10),
        cmocka_unit_test(libspdm_test_requester_get_digests_case11),
        cmocka_unit_test(libspdm_test_requester_get_digests_case12),
        cmocka_unit_test(libspdm_test_requester_get_digests_case13),
        cmocka_unit_test(libspdm_test_requester_get_digests_case14),
        cmocka_unit_test(libspdm_test_requester_get_digests_case15),
        cmocka_unit_test(libspdm_test_requester_get_digests_case16),
        cmocka_unit_test(libspdm_test_requester_get_digests_case17),
        cmocka_unit_test(libspdm_test_requester_get_digests_case18),
        cmocka_unit_test(libspdm_test_requester_get_digests_case19),
        cmocka_unit_test(libspdm_test_requester_get_digests_case20),
        cmocka_unit_test(libspdm_test_requester_get_digests_case21),
        cmocka_unit_test(libspdm_test_requester_get_digests_case22),
        cmocka_unit_test(libspdm_test_requester_get_digests_case23),
        cmocka_unit_test(libspdm_test_requester_get_digests_case24),
        cmocka_unit_test(libspdm_test_requester_get_digests_case25),
    };

    libspdm_setup_test_context(&m_libspdm_requester_get_digests_test_context);

    return cmocka_run_group_tests(spdm_requester_get_digests_tests,
                                  libspdm_unit_test_group_setup,
                                  libspdm_unit_test_group_teardown);
}

#endif /* LIBSPDM_ENABLE_CAPABILITY_CERT_CAP */
