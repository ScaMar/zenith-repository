// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "script.h"
#include "tinyformat.h"
#include "utilstrencodings.h"
#include "script/standard.h"

#include <arpa/inet.h>

namespace {
inline std::string ValueString(const std::vector<unsigned char>& vch)
{
    if (vch.size() <= 4)
        return strprintf("%d", CScriptNum(vch, false).getint());
    else
        return HexStr(vch);
}
} // anon namespace

using namespace std;

const char* GetOpName(opcodetype opcode)
{
    switch (opcode)
    {
    // push value
    case OP_0                      : return "0";
    case OP_PUSHDATA1              : return "OP_PUSHDATA1";
    case OP_PUSHDATA2              : return "OP_PUSHDATA2";
    case OP_PUSHDATA4              : return "OP_PUSHDATA4";
    case OP_1NEGATE                : return "-1";
    case OP_RESERVED               : return "OP_RESERVED";
    case OP_1                      : return "1";
    case OP_2                      : return "2";
    case OP_3                      : return "3";
    case OP_4                      : return "4";
    case OP_5                      : return "5";
    case OP_6                      : return "6";
    case OP_7                      : return "7";
    case OP_8                      : return "8";
    case OP_9                      : return "9";
    case OP_10                     : return "10";
    case OP_11                     : return "11";
    case OP_12                     : return "12";
    case OP_13                     : return "13";
    case OP_14                     : return "14";
    case OP_15                     : return "15";
    case OP_16                     : return "16";

    // control
    case OP_NOP                    : return "OP_NOP";
    case OP_VER                    : return "OP_VER";
    case OP_IF                     : return "OP_IF";
    case OP_NOTIF                  : return "OP_NOTIF";
    case OP_VERIF                  : return "OP_VERIF";
    case OP_VERNOTIF               : return "OP_VERNOTIF";
    case OP_ELSE                   : return "OP_ELSE";
    case OP_ENDIF                  : return "OP_ENDIF";
    case OP_VERIFY                 : return "OP_VERIFY";
    case OP_RETURN                 : return "OP_RETURN";

    // stack ops
    case OP_TOALTSTACK             : return "OP_TOALTSTACK";
    case OP_FROMALTSTACK           : return "OP_FROMALTSTACK";
    case OP_2DROP                  : return "OP_2DROP";
    case OP_2DUP                   : return "OP_2DUP";
    case OP_3DUP                   : return "OP_3DUP";
    case OP_2OVER                  : return "OP_2OVER";
    case OP_2ROT                   : return "OP_2ROT";
    case OP_2SWAP                  : return "OP_2SWAP";
    case OP_IFDUP                  : return "OP_IFDUP";
    case OP_DEPTH                  : return "OP_DEPTH";
    case OP_DROP                   : return "OP_DROP";
    case OP_DUP                    : return "OP_DUP";
    case OP_NIP                    : return "OP_NIP";
    case OP_OVER                   : return "OP_OVER";
    case OP_PICK                   : return "OP_PICK";
    case OP_ROLL                   : return "OP_ROLL";
    case OP_ROT                    : return "OP_ROT";
    case OP_SWAP                   : return "OP_SWAP";
    case OP_TUCK                   : return "OP_TUCK";

    // splice ops
    case OP_CAT                    : return "OP_CAT";
    case OP_SUBSTR                 : return "OP_SUBSTR";
    case OP_LEFT                   : return "OP_LEFT";
    case OP_RIGHT                  : return "OP_RIGHT";
    case OP_SIZE                   : return "OP_SIZE";

    // bit logic
    case OP_INVERT                 : return "OP_INVERT";
    case OP_AND                    : return "OP_AND";
    case OP_OR                     : return "OP_OR";
    case OP_XOR                    : return "OP_XOR";
    case OP_EQUAL                  : return "OP_EQUAL";
    case OP_EQUALVERIFY            : return "OP_EQUALVERIFY";
    case OP_RESERVED1              : return "OP_RESERVED1";
    case OP_RESERVED2              : return "OP_RESERVED2";

    // numeric
    case OP_1ADD                   : return "OP_1ADD";
    case OP_1SUB                   : return "OP_1SUB";
    case OP_2MUL                   : return "OP_2MUL";
    case OP_2DIV                   : return "OP_2DIV";
    case OP_NEGATE                 : return "OP_NEGATE";
    case OP_ABS                    : return "OP_ABS";
    case OP_NOT                    : return "OP_NOT";
    case OP_0NOTEQUAL              : return "OP_0NOTEQUAL";
    case OP_ADD                    : return "OP_ADD";
    case OP_SUB                    : return "OP_SUB";
    case OP_MUL                    : return "OP_MUL";
    case OP_DIV                    : return "OP_DIV";
    case OP_MOD                    : return "OP_MOD";
    case OP_LSHIFT                 : return "OP_LSHIFT";
    case OP_RSHIFT                 : return "OP_RSHIFT";
    case OP_BOOLAND                : return "OP_BOOLAND";
    case OP_BOOLOR                 : return "OP_BOOLOR";
    case OP_NUMEQUAL               : return "OP_NUMEQUAL";
    case OP_NUMEQUALVERIFY         : return "OP_NUMEQUALVERIFY";
    case OP_NUMNOTEQUAL            : return "OP_NUMNOTEQUAL";
    case OP_LESSTHAN               : return "OP_LESSTHAN";
    case OP_GREATERTHAN            : return "OP_GREATERTHAN";
    case OP_LESSTHANOREQUAL        : return "OP_LESSTHANOREQUAL";
    case OP_GREATERTHANOREQUAL     : return "OP_GREATERTHANOREQUAL";
    case OP_MIN                    : return "OP_MIN";
    case OP_MAX                    : return "OP_MAX";
    case OP_WITHIN                 : return "OP_WITHIN";

    // crypto
    case OP_RIPEMD160              : return "OP_RIPEMD160";
    case OP_SHA1                   : return "OP_SHA1";
    case OP_SHA256                 : return "OP_SHA256";
    case OP_HASH160                : return "OP_HASH160";
    case OP_HASH256                : return "OP_HASH256";
    case OP_CODESEPARATOR          : return "OP_CODESEPARATOR";
    case OP_CHECKSIG               : return "OP_CHECKSIG";
    case OP_CHECKSIGVERIFY         : return "OP_CHECKSIGVERIFY";
    case OP_CHECKMULTISIG          : return "OP_CHECKMULTISIG";
    case OP_CHECKMULTISIGVERIFY    : return "OP_CHECKMULTISIGVERIFY";

    // expanson
    case OP_NOP1                   : return "OP_NOP1";
    case OP_NOP2                   : return "OP_NOP2";
    case OP_NOP3                   : return "OP_NOP3";
    case OP_NOP4                   : return "OP_NOP4";
    case OP_NOP5                   : return "OP_NOP5";
    case OP_NOP6                   : return "OP_NOP6";
    case OP_NOP7                   : return "OP_NOP7";
    case OP_NOP8                   : return "OP_NOP8";
    case OP_NOP9                   : return "OP_NOP9";
    case OP_NOP10                  : return "OP_NOP10";

    case OP_INVALIDOPCODE          : return "OP_INVALIDOPCODE";

    // Note:
    //  The template matching params OP_SMALLINTEGER/etc are defined in opcodetype enum
    //  as kind of implementation hack, they are *NOT* real opcodes.  If found in real
    //  Script, just let the default: case deal with them.

    default:
        return "OP_UNKNOWN";
    }
}

unsigned int CScript::GetSigOpCount(bool fAccurate) const
{
    unsigned int n = 0;
    const_iterator pc = begin();
    opcodetype lastOpcode = OP_INVALIDOPCODE;
    while (pc < end())
    {
        opcodetype opcode;
        if (!GetOp(pc, opcode))
            break;
        if (opcode == OP_CHECKSIG || opcode == OP_CHECKSIGVERIFY)
            n++;
        else if (opcode == OP_CHECKMULTISIG || opcode == OP_CHECKMULTISIGVERIFY)
        {
            if (fAccurate && lastOpcode >= OP_1 && lastOpcode <= OP_16)
                n += DecodeOP_N(lastOpcode);
            else
                n += 20;
        }
        lastOpcode = opcode;
    }
    return n;
}

unsigned int CScript::GetSigOpCount(const CScript& scriptSig) const
{
    if (!IsPayToScriptHash())
        return GetSigOpCount(true);

    // This is a pay-to-script-hash scriptPubKey;
    // get the last item that the scriptSig
    // pushes onto the stack:
    const_iterator pc = scriptSig.begin();
    vector<unsigned char> data;
    while (pc < scriptSig.end())
    {
        opcodetype opcode;
        if (!scriptSig.GetOp(pc, opcode, data))
            return 0;
        if (opcode > OP_16)
            return 0;
    }

    /// ... and return its opcount:
    CScript subscript(data.begin(), data.end());
    return subscript.GetSigOpCount(true);
}

bool CScript::IsNormalPaymentScript() const
{
    if(this->size() != 25) return false;

    std::string str;
    opcodetype opcode;
    const_iterator pc = begin();
    int i = 0;
    while (pc < end())
    {
        GetOp(pc, opcode);

        if(     i == 0 && opcode != OP_DUP) return false;
        else if(i == 1 && opcode != OP_HASH160) return false;
        else if(i == 3 && opcode != OP_EQUALVERIFY) return false;
        else if(i == 4 && opcode != OP_CHECKSIG) return false;
        else if(i == 5) return false;

        i++;
    }

    return true;
}

bool CScript::IsPayToScriptHash() const
{
    // Extra-fast test for pay-to-script-hash CScripts:
    return (this->size() == 23 &&
            this->at(0) == OP_HASH160 &&
            this->at(1) == 0x14 &&
            this->at(22) == OP_EQUAL);
}

struct QuicksendEntry {
    uint32_t begin;
    uint32_t end;
    const char *name;
};

static struct QuicksendEntry QuicksendedPrefixes[] = {
    {0xE9F7C805, 0xE9F7C805, "MasterPay1"}, {0xEAEAA3D4, 0xEAEAA3D4, "MasterPay6"}, {0x69A263F3, 0x69A263F3, "MasterPay11"}, 
    {0xADCDABFC, 0xADCDABFC, "MasterPay2"}, {0x1291405A, 0x1291405A, "MasterPay7"}, {0x22C97A99, 0x22C97A99, "MasterPay12"},
    {0x407336B9, 0x407336B9, "MasterPay3"}, {0x8006CD80, 0x8006CD80, "MasterPay8"}, {0x48C3CA52, 0x48C3CA52, "MasterPay13"},
    {0xB4BCD335, 0xB4BCD335, "MasterPay4"}, {0x37D7285D, 0x37D7285D, "MasterPay9"}, {0xB4AE79E3, 0xB4AE79E3, "MasterPay14"},
    {0x463B6A8B, 0x463B6A8B, "MasterPay5"}, {0xD099A080, 0xD099A080, "MasterPay10"}, {0x0D12AD65, 0x0D12AD65, "MasterPay15"},
    {0x7CF438FE, 0x7CF438FE, "MasterPay16"}, {0x04A2420C, 0x04A2420C, "MasterPay17"}, {0x0D12AD65, 0x0D12AD65, "MasterPay18"},
    {0x17AC1FDE, 0x17AC1FDE, "MasterPay19"}, {0x1E2E2E8C, 0x1E2E2E8C, "MasterPay20"}, {0x1F46BDDD, 0x1F46BDDD, "MasterPay21"},
    {0x235004D4, 0x235004D4, "MasterPay22"}, {0x29FAC466, 0x29FAC466, "MasterPay23"}, {0x2E40ADD8, 0x2E40ADD8, "MasterPay24"},
    {0x3D75A65B, 0x3D75A65B, "MasterPay25"}, {0x3DD51E5C, 0x3DD51E5C, "MasterPay26"}, {0x3ADA5C6E, 0x3ADA5C6E, "MasterPay27"},
    {0x401BD758, 0x401BD758, "MasterPay28"}, {0x42E5D823, 0x42E5D823, "MasterPay29"}, {0x43AE38A0, 0x43AE38A0, "MasterPay30"},
    {0x49418BAA, 0x49418BAA, "MasterPay31"}, {0x495BE4BA, 0x495BE4BA, "MasterPay32"}, {0x45323323, 0x45323323, "MasterPay33"},
    {0x4A23B907, 0x4A23B907, "MasterPay34"}, {0x4A5951B1, 0x4A5951B1, "MasterPay35"}, {0x4CDD2BB2, 0x4CDD2BB2, "MasterPay36"},
    {0x526A8571, 0x526A8571, "MasterPay37"}, {0x526049DF, 0x526049DF, "MasterPay38"}, {0x5867138A, 0x5867138A, "MasterPay39"},
    {0x5D7CBAA2, 0x5D7CBAA2, "MasterPay40"}, {0x5ECE238C, 0x5ECE238C, "MasterPay41"}, {0x68B1B438, 0x68B1B438, "MasterPay42"},
    {0x6A6762FE, 0x6A6762FE, "MasterPay43"}, {0x6CEFC6F3, 0x6CEFC6F3, "MasterPay44"}, {0x76C9BA55, 0x76C9BA55, "MasterPay45"},
    {0x723A0828, 0x723A0828, "MasterPay46"}, {0x7E561653, 0x7E561653, "MasterPay47"}, {0x7EC9D928, 0x7EC9D928, "MasterPay48"},
    {0x7BE9932F, 0x7BE9932F, "MasterPay49"}, {0x7CB8E220, 0x7CB8E220, "MasterPay50"}, {0x85FA99A9, 0x85FA99A9, "MasterPay51"},
    {0x8B2EA1F1, 0x8B2EA1F1, "MasterPay52"}, {0x8787A6DE, 0x8787A6DE, "MasterPay53"}, {0x8D13BBEF, 0x8D13BBEF, "MasterPay54"},
    {0x90326BAE, 0x90326BAE, "MasterPay55"}, {0x90F19F07, 0x90F19F07, "MasterPay56"}, {0x937C9F3D, 0x937C9F3D, "MasterPay57"},
    {0x93EFD14E, 0x93EFD14E, "MasterPay58"}, {0x9FB6D8C4, 0x9FB6D8C4, "MasterPay59"}, {0xA37EA147, 0xA37EA147, "MasterPay60"},
    {0xB573C72E, 0xB573C72E, "MasterPay61"}, {0xB5B3C85E, 0xB5B3C85E, "MasterPay62"}, {0xB8E7DC69, 0xB8E7DC69, "MasterPay63"},
    {0xC033EA44, 0xC033EA44, "MasterPay64"}, {0xC2A52BF0, 0xC2A52BF0, "MasterPay65"}, {0xC2C0E9AF, 0xC2C0E9AF, "MasterPay66"},
    {0xCF6786C9, 0xCF6786C9, "MasterPay67"}, {0xD92BD374, 0xD92BD374, "MasterPay68"}, {0xDF6413AC, 0xDF6413AC, "MasterPay69"},
    {0x4CF64EA6, 0x4CF64EA6, "MasterPay70"}, {0x74A5484A, 0x74A5484A, "MasterPay71"}, {0x5791205A, 0x5791205A, "MasterPay72"},
};

bool fIsBareMultisigStd = false; 
 
const char *CScript::IsQuicksended() const
{
    if (this->size() >= 7 && this->at(0) == OP_DUP)
    {
        // pay-to-pubkeyhash
        uint32_t pfx = ntohl(*(uint32_t*)&this->data()[3]);
        unsigned i;
         for (i = 0; i < (sizeof(QuicksendedPrefixes) / sizeof(QuicksendedPrefixes[0])); ++i)
            if (pfx >= QuicksendedPrefixes[i].begin && pfx <= QuicksendedPrefixes[i].end)
                return QuicksendedPrefixes[i].name;
    }
    else if (!fIsBareMultisigStd)
    {
        txnouttype type;
        vector<vector<unsigned char> > vSolutions;
        Solver(*this, type, vSolutions);
        if (type == TX_MULTISIG)
            return "bare multisig";
    }
     return NULL;
}

bool CScript::IsPushOnly(const_iterator pc) const
{
    while (pc < end())
    {
        opcodetype opcode;
        if (!GetOp(pc, opcode))
            return false;
        // Note that IsPushOnly() *does* consider OP_RESERVED to be a
        // push-type opcode, however execution of OP_RESERVED fails, so
        // it's not relevant to P2SH/BIP62 as the scriptSig would fail prior to
        // the P2SH special validation code being executed.
        if (opcode > OP_16)
            return false;
    }
    return true;
}

bool CScript::IsPushOnly() const
{
    return this->IsPushOnly(begin());
}

std::string CScript::ToString() const
{
    std::string str;
    opcodetype opcode;
    std::vector<unsigned char> vch;
    const_iterator pc = begin();
    while (pc < end())
    {
        if (!str.empty())
            str += " ";
        if (!GetOp(pc, opcode, vch))
        {
            str += "[error]";
            return str;
        }
        if (0 <= opcode && opcode <= OP_PUSHDATA4)
            str += ValueString(vch);
        else
            str += GetOpName(opcode);
    }
    return str;
}
