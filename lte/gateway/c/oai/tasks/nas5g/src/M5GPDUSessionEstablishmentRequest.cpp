/*
   Copyright 2020 The Magma Authors.
   This source code is licensed under the BSD-style license found in the
   LICENSE file in the root directory of this source tree.
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#include <sstream>
#include "M5GPDUSessionEstablishmentRequest.h"
#include "M5GCommonDefs.h"

namespace magma5g {
PDUSessionEstablishmentRequestMsg::PDUSessionEstablishmentRequestMsg(){};
PDUSessionEstablishmentRequestMsg::~PDUSessionEstablishmentRequestMsg(){};

// Decode PDUSessionEstablishmentRequest Message and its IEs
int PDUSessionEstablishmentRequestMsg::DecodePDUSessionEstablishmentRequestMsg(
    PDUSessionEstablishmentRequestMsg* pdu_session_estab_request,
    uint8_t* buffer, uint32_t len) {
  uint32_t decoded   = 0;
  int decoded_result = 0;
  uint8_t type_len   = sizeof(uint8_t);
  uint8_t length_len = sizeof(uint8_t);

  CHECK_PDU_POINTER_AND_LENGTH_DECODER(
      buffer, PDU_SESSION_ESTABLISH_REQ_MIN_LEN, len);

  MLOG(MDEBUG) << "DecodePDUSessionEstablishmentRequestMsg : ";
  if ((decoded_result =
           pdu_session_estab_request->extended_protocol_discriminator
               .DecodeExtendedProtocolDiscriminatorMsg(
                   &pdu_session_estab_request->extended_protocol_discriminator,
                   0, buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;
  if ((decoded_result =
           pdu_session_estab_request->pdu_session_identity
               .DecodePDUSessionIdentityMsg(
                   &pdu_session_estab_request->pdu_session_identity, 0,
                   buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;
  if ((decoded_result = pdu_session_estab_request->pti.DecodePTIMsg(
           &pdu_session_estab_request->pti, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;
  if ((decoded_result =
           pdu_session_estab_request->message_type.DecodeMessageTypeMsg(
               &pdu_session_estab_request->message_type, 0, buffer + decoded,
               len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;
  if ((decoded_result =
           pdu_session_estab_request->integrity_prot_max_data_rate
               .DecodeIntegrityProtMaxDataRateMsg(
                   &pdu_session_estab_request->integrity_prot_max_data_rate, 0,
                   buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;
  if ((decoded_result =
           pdu_session_estab_request->pdu_session_type.DecodePDUSessionTypeMsg(
               &pdu_session_estab_request->pdu_session_type, PDU_SESSION_TYPE,
               buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;
  if ((decoded_result = pdu_session_estab_request->ssc_mode.DecodeSSCModeMsg(
           &pdu_session_estab_request->ssc_mode, SSC_MODE, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  while (decoded < len) {
    // Size is incremented for the unhandled types by 1 byte
    uint32_t type = *(buffer + decoded) >= 0x80 ?
                        ((*(buffer + decoded)) & 0xf0) :
                        (*(buffer + decoded));
    decoded_result = 0;

    switch (type) {
      case REQUEST_5GSM_CAPABILITY_TYPE:
      case REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_TYPE:
      case REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_TYPE:
      case REQUEST_SM_PDU_DN_REQUEST_CONTAINER_TYPE:
      case REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
      case REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE:
      case REQUEST_DS_TT_ETHERNET_PORT_MAC_ADDRESS_TYPE:
      case REQUEST_UE_DS_TT_RESIDENCE_TIME_TYPE:
      case REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE:

        // TLV Types. 1 byte for Type and 1 Byte for size
        type_len = sizeof(uint8_t);

        if (type == REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE) {
          length_len = 2 * sizeof(uint8_t);
          DECODE_U16(buffer + decoded + type_len, decoded_result, decoded);
        } else {
          length_len = sizeof(uint8_t);
          DECODE_U8(buffer + decoded + type_len, decoded_result, decoded);
        }

        decoded += (length_len + decoded_result);
        break;

      default:
        decoded_result = -1;
        break;
    }

    if (decoded_result < 0) {
      return decoded_result;
    }
  }

  return decoded;
}

// Encode PDUSessionEstablishmentRequest Message and its IEs
int PDUSessionEstablishmentRequestMsg::EncodePDUSessionEstablishmentRequestMsg(
    PDUSessionEstablishmentRequestMsg* pdu_session_estab_request,
    uint8_t* buffer, uint32_t len) {
  // Not yet implemented, will be supported POST MVC
  return 0;
}
}  // namespace magma5g
