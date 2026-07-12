#include <bcrypt.h>



int encrypt_message(BCRYPT_KEY_HANDLE aesKey, const BYTE *nonce, int nonce_len, const BYTE *plaintext, int plaintext_len, BYTE *ciphertext, int ciphertext_buffer_len, int *ciphertext_len, BYTE *tag, int tag_len);
int decrypt_message(BCRYPT_KEY_HANDLE aesKey, const BYTE *nonce, int nonce_len, const BYTE *plaintext, int plaintext_len, BYTE *ciphertext, int ciphertext_buffer_len, int *ciphertext_len, BYTE *tag, int tag_len, BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO *pPaddingInfo);
int encrypt_message(BCRYPT_KEY_HANDLE aesKey, const BYTE *nonce, int nonce_len, const BYTE *plaintext, int plaintext_len, BYTE *ciphertext, int ciphertext_buffer_len, int *ciphertext_len, BYTE *tag, int tag_len)
{

    // Generate new nonce (need to create new nonce for each send)
    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
    BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
    authInfo.pbNonce = (PUCHAR)nonce;
    authInfo.cbNonce = nonce_len;
    authInfo.pbTag = tag;
    authInfo.cbTag = tag_len;

    ULONG resultlen = 0;

    // encrypt message
    NTSTATUS status = BCryptEncrypt(
        aesKey,
        (PUCHAR)plaintext,
        plaintext_len,
        &authInfo,
        (PUCHAR)nonce,
        nonce_len,
        ciphertext,
        ciphertext_buffer_len,
        &resultlen,
        0);

    if (!BCRYPT_SUCCESS(status))
        return 0;

    *ciphertext_len = (int)resultlen;
    return 1;
}

int decrypt_message(BCRYPT_KEY_HANDLE aesKey, const BYTE *nonce, int nonce_len, const BYTE *plaintext, int plaintext_len, BYTE *ciphertext, int ciphertext_buffer_len, int *ciphertext_len, BYTE *tag, int tag_len, BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO *pPaddingInfo)
{
    ULONG resultlen = 0;
    //decrypt message
    NTSTATUS status = BCryptDecrypt(
        aesKey,
        (PUCHAR)plaintext,
        plaintext_len,
        &pPaddingInfo,
        (PUCHAR)nonce,
        nonce_len,
        ciphertext,
        ciphertext_buffer_len,
        &resultlen,
        0);

    if (!BCRYPT_SUCCESS(status))
        return 0;

    *ciphertext_len = (int)resultlen;
    return 1;
}