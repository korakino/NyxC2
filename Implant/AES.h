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
        NULL,
        0,
        ciphertext,
        ciphertext_buffer_len,
        &resultlen,
        0);

    if (!BCRYPT_SUCCESS(status))
        return 0;

    *ciphertext_len = (int)resultlen;
    return 1;
}

int decrypt_message(BCRYPT_KEY_HANDLE aesKey,
                    const BYTE *nonce, int nonce_len,
                    const BYTE *tag, int tag_len,
                    const BYTE *ciphertext, int ciphertext_len,
                    BYTE *plaintext, int plaintext_buffer_len, int *plaintext_len)
{
    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
    BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
    authInfo.pbNonce = (PUCHAR)nonce;
    authInfo.cbNonce = nonce_len;
    authInfo.pbTag = (PUCHAR)tag;
    authInfo.cbTag = tag_len;

    ULONG resultlen = 0;

    NTSTATUS status = BCryptDecrypt(
        aesKey,
        (PUCHAR)ciphertext,
        ciphertext_len,
        &authInfo,
        NULL,
        0,
        plaintext,
        plaintext_buffer_len,
        &resultlen,
        0);

    if (!BCRYPT_SUCCESS(status))
        return 0;

    *plaintext_len = (int)resultlen;
    return 1;
}