# import cryptography
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding
import os
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
import struct


def BCRYPT_RSAPUBLIC_BLOB(public_key):
    public_numbers = public_key.public_numbers()

    modulus = public_numbers.n.to_bytes(
        (public_numbers.n.bit_length() + 7) // 8,
        "big",
    )
    exponent = public_numbers.e.to_bytes(
        (public_numbers.e.bit_length() + 7) // 8,
        "big",
    )

    header = struct.pack(
        "<LLLLLL",
        0x31415352,
        public_numbers.n.bit_length(),
        len(exponent),
        len(modulus),
        0,
        0,
    )

    return header + exponent + modulus



def rsa_key_setup():
    #setting up rsa encryption
    private_key = rsa.generate_private_key(
    public_exponent=65537,
    key_size=2048,
)
    public_key = private_key.public_key()

    blob = BCRYPT_RSAPUBLIC_BLOB(public_key)

    
    return private_key,blob


def rsa_decryption(private_key, ciphertext):
    return private_key.decrypt(
    ciphertext,
    padding.OAEP(
        mgf=padding.MGF1(algorithm=hashes.SHA256()),
        algorithm=hashes.SHA256(),
        label=None
    )
)








def aes_encrypt(key : bytes, plaintext : str):
    
    nonce = os.urandom(12)
    cipher = Cipher(algorithms.AES(key), modes.GCM(nonce))
    encryptor = cipher.encryptor()
    ct = encryptor.update(bytes(f"{plaintext}")) + encryptor.finalize()
    payload = nonce + encryptor.tag + ct
    return payload

def aes_decrypt(key : bytes, payload: bytes):
    
    nonce = payload[:12]
    tag = payload[12:28]
    ciphertext = payload[28:]
    
    decryptor = Cipher(algorithms.AES(key), modes.GCM(nonce, tag)).decryptor()
    plaintext = decryptor.update(ciphertext) + decryptor.finalize()
    
    return plaintext
    

