# import cryptography
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding
import os
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes


def rsa_key_setup():
    #setting up rsa encryption
    private_key = rsa.generate_private_key(
    public_exponent=65537,
    key_size=2048,
)
    public_key = private_key.public_key()
    
    
    #usable public_key to send messages
    pem = public_key.public_bytes(
   encoding=serialization.Encoding.PEM,
   format=serialization.PublicFormat.SubjectPublicKeyInfo
)   
    return private_key,pem


def rsa_decryption(private_key, ciphertext):
    plaintext = private_key.decrypt(
    ciphertext,
    padding.OAEP(
        mgf=padding.MGF1(algorithm=hashes.SHA256()),
        algorithm=hashes.SHA256(),
        label=None
    )
)
    return 


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
    

