/**
 *  Copyright Notice:
 *  Copyright 2021-2022 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

#include "spdm_unit_test.h"
#include "internal/libspdm_requester_lib.h"

#if (LIBSPDM_ENABLE_CAPABILITY_MUT_AUTH_CAP) || (LIBSPDM_ENABLE_CAPABILITY_ENCAP_CAP)

spdm_challenge_request_t m_spdm_challenge_request1 = {
    {SPDM_MESSAGE_VERSION_11, SPDM_CHALLENGE, 0,
     SPDM_CHALLENGE_REQUEST_NO_MEASUREMENT_SUMMARY_HASH},
};
size_t m_spdm_challenge_request1_size = sizeof(m_spdm_challenge_request1);

spdm_challenge_request_t m_spdm_challenge_request3 = {
    {SPDM_MESSAGE_VERSION_11, SPDM_CHALLENGE, SPDM_MAX_SLOT_COUNT,
     SPDM_CHALLENGE_REQUEST_NO_MEASUREMENT_SUMMARY_HASH},
};
size_t m_spdm_challenge_request3_size = sizeof(m_spdm_challenge_request3);

spdm_challenge_request_t m_spdm_challenge_request4 = {
    {SPDM_MESSAGE_VERSION_11, SPDM_CHALLENGE, 0xFF,
     SPDM_CHALLENGE_REQUEST_NO_MEASUREMENT_SUMMARY_HASH},
};
size_t m_spdm_challenge_request4_size = sizeof(m_spdm_challenge_request4);

/**
 * Test 1: receiving a correct CHALLENGE message from the requester with
 * no opaque data, no measurements, and slot number 0.
 * Expected behavior: the requester accepts the request and produces a valid
 * CHALLENGE_AUTH response message.
 **/
void test_libspdm_requester_encap_challenge_auth_case1(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_SPDM_MSG_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void *data;
    size_t data_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x1;

    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP;
    spdm_context->connection_info.capability.flags = 0;

    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo =
        m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.algorithm.req_base_asym_alg =
        SPDM_ALGORITHMS_BASE_ASYM_ALGO_TPM_ALG_RSASSA_2048;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11
                                            << SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data,
                                                    &data_size, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size;

    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    spdm_context->transcript.message_m.buffer_size =
        spdm_context->transcript.message_m.max_buffer_size;
#endif

    response_size = sizeof(response);
    libspdm_get_random_number(SPDM_NONCE_SIZE,
                              m_spdm_challenge_request1.nonce);
    status = libspdm_get_encap_response_challenge_auth(
        spdm_context, m_spdm_challenge_request1_size,
        &m_spdm_challenge_request1, &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_challenge_auth_response_t) +
                     libspdm_get_hash_size(spdm_context->connection_info.algorithm.base_hash_algo) +
                     SPDM_NONCE_SIZE + 0 +
                     sizeof(uint16_t) +
                     spdm_context->local_context.opaque_challenge_auth_rsp_size +
                     libspdm_get_req_asym_signature_size(
                         spdm_context->connection_info.algorithm.req_base_asym_alg));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_CHALLENGE_AUTH);
    assert_int_equal(spdm_response->header.param1, 0);
    assert_int_equal(spdm_response->header.param2, 1 << 0);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(spdm_context->transcript.message_m.buffer_size,
                     0);
#endif
    free(data);
}

/**
 * Test 2:
 * Expected behavior:
 **/
void test_libspdm_requester_encap_challenge_auth_case2(void **state)
{
}

/**
 * Test 3: receiving a correct CHALLENGE from the requester, but the requester does not
 * have the challenge capability set.
 * Expected behavior: the requester accepts the request and produces a valid
 * CHALLENGE_AUTH response message.
 **/
void test_libspdm_requester_encap_challenge_auth_case3(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_SPDM_MSG_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void *data;
    size_t data_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x3;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    /* spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;*/
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11
                                            << SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data, &data_size,
                                                    NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size;

    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number(SPDM_NONCE_SIZE, m_spdm_challenge_request1.nonce);
    status = libspdm_get_encap_response_challenge_auth(spdm_context, m_spdm_challenge_request1_size,
                                                       &m_spdm_challenge_request1, &response_size,
                                                       response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code, SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1, SPDM_ERROR_CODE_UNSUPPORTED_REQUEST);
    assert_int_equal(spdm_response->header.param2, SPDM_CHALLENGE);
    free(data);
}

/**
 * Test 4: receiving an incorrect CHALLENGE from the requester, with the slot number
 * larger than the specification limit.
 * Expected behavior: the requester rejects the request, and produces an ERROR message
 * indicating the UnexpectedRequest.
 **/
void test_libspdm_requester_encap_challenge_auth_case4(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_SPDM_MSG_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void *data;
    size_t data_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x4;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11
                                            << SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data, &data_size,
                                                    NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size;

    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number(SPDM_NONCE_SIZE, m_spdm_challenge_request1.nonce);
    status = libspdm_get_encap_response_challenge_auth(spdm_context, m_spdm_challenge_request3_size,
                                                       &m_spdm_challenge_request3, &response_size,
                                                       response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code, SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1, SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    free(data);
}

/**
 * Test 5: receiving a correct CHALLENGE from the requester, but with certificate
 * unavailable at the requested slot number (1).
 * Expected behavior: the requester rejects the request, and produces an ERROR message
 * indicating the UnexpectedRequest.
 **/
void test_libspdm_requester_encap_challenge_auth_case5(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_SPDM_MSG_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void *data;
    size_t data_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x05;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11
                                            << SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data, &data_size,
                                                    NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size;

    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number(SPDM_NONCE_SIZE, m_spdm_challenge_request1.nonce);
    status = libspdm_get_encap_response_challenge_auth(spdm_context, m_spdm_challenge_request3_size,
                                                       &m_spdm_challenge_request3, &response_size,
                                                       response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code, SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1, SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    free(data);
}

/**
 * Test 6: receiving a correct CHALLENGE message from the requester with
 * no opaque data, no measurements, and slot number 0xFF.
 * Expected behavior: the requester accepts the request and produces a valid
 * CHALLENGE_AUTH response message using provisioned public key (slot number 0xFF).
 **/
void test_libspdm_requester_encap_challenge_auth_case6(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_SPDM_MSG_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void *data;
    size_t data_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x6;

    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CHAL_CAP;

    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.req_base_asym_alg = m_libspdm_use_req_asym_algo;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11
                                            << SPDM_VERSION_NUMBER_SHIFT_BIT;

    libspdm_read_requester_public_key(m_libspdm_use_req_asym_algo, &data, &data_size);
    spdm_context->local_context.local_public_key_provision = data;
    spdm_context->local_context.local_public_key_provision_size = data_size;

    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    spdm_context->transcript.message_m.buffer_size =
        spdm_context->transcript.message_m.max_buffer_size;
#endif

    response_size = sizeof(response);
    libspdm_get_random_number(SPDM_NONCE_SIZE, m_spdm_challenge_request4.nonce);
    status = libspdm_get_encap_response_challenge_auth(
        spdm_context,
        m_spdm_challenge_request4_size, &m_spdm_challenge_request4,
        &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(
        response_size,
        sizeof(spdm_challenge_auth_response_t) +
        libspdm_get_hash_size(spdm_context->connection_info.algorithm.base_hash_algo) +
        SPDM_NONCE_SIZE + 0 +
        sizeof(uint16_t) + 0 +
        libspdm_get_req_asym_signature_size(
            spdm_context->connection_info.algorithm.req_base_asym_alg));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code, SPDM_CHALLENGE_AUTH);
    assert_int_equal(spdm_response->header.param1, 0xF);
    assert_int_equal(spdm_response->header.param2, 0);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(spdm_context->transcript.message_m.buffer_size, 0);
#endif
    free(data);
}

libspdm_test_context_t m_spdm_requester_challenge_auth_test_context = {
    LIBSPDM_TEST_CONTEXT_VERSION,
    false,
};

int libspdm_requester_encap_challenge_auth_test_main(void)
{
    const struct CMUnitTest spdm_requester_challenge_auth_tests[] = {
        /* Success Case*/
        cmocka_unit_test(test_libspdm_requester_encap_challenge_auth_case1),
        /* Can be populated with new test.*/
        cmocka_unit_test(test_libspdm_requester_encap_challenge_auth_case2),
        /* connection_state Check*/
        cmocka_unit_test(test_libspdm_requester_encap_challenge_auth_case3),
        cmocka_unit_test(test_libspdm_requester_encap_challenge_auth_case4),
        cmocka_unit_test(test_libspdm_requester_encap_challenge_auth_case5),
        /* Success Case, use provisioned public key (slot 0xFF) */
        cmocka_unit_test(test_libspdm_requester_encap_challenge_auth_case6),
    };

    libspdm_setup_test_context(&m_spdm_requester_challenge_auth_test_context);

    return cmocka_run_group_tests(spdm_requester_challenge_auth_tests,
                                  libspdm_unit_test_group_setup,
                                  libspdm_unit_test_group_teardown);
}

#endif /* (LIBSPDM_ENABLE_CAPABILITY_MUT_AUTH_CAP) || (LIBSPDM_ENABLE_CAPABILITY_ENCAP_CAP)*/
