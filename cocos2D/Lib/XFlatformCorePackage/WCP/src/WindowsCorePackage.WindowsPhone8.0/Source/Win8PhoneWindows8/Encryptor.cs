﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security;
using System.IO;

#if OS_W8
using Windows.Security.Cryptography;
using Windows.Security.Cryptography.Core;
using Windows.Storage.Streams;
#else
using System.Security.Cryptography;
#endif

namespace WCPToolkit
{
    namespace Utils
    {
        namespace Encryption
        {
            public enum EncryptionType
            {
                EET_B64BASE,
                EET_AES,
                EET_TOTAL
            }

            public sealed class Encryptor
            {
                public static string Encrypt(string input, EncryptionType encription)
                {
                    switch (encription)
                    {
                        case EncryptionType.EET_B64BASE:
                            return EncryptB64(input, true);
                        case EncryptionType.EET_AES:
                            return EncryptAES(input, true);
                        default:
                            return "Unsupported Encryption Selected";
                    }
                }
                public static string Decrypt(string input, EncryptionType encription)
                {
                    switch (encription)
                    {
                        case EncryptionType.EET_B64BASE:
                            return EncryptB64(input, false);
                        case EncryptionType.EET_AES:
                            return EncryptAES(input, false);
                        default:
                            return "Unsupported Encryption Selected";
                    }
                }

                static string EncryptB64(string input, bool bEncrypt)
                {
                    string rez = "";
                    if (bEncrypt)
                    {
                        try
                        {
                            byte[] encData = new byte[input.Length];
                            encData = System.Text.Encoding.UTF8.GetBytes(input);
                            rez = Convert.ToBase64String(encData);
                        }
                        catch (Exception e)
                        {
                            rez = "Exception in b64Base Encoding " + e.ToString();
                        }
                    }
                    else
                    {
                        try
                        {
                            System.Text.UTF8Encoding encoder = new System.Text.UTF8Encoding();
                            System.Text.Decoder decoder = encoder.GetDecoder();

                            byte[] decData = Convert.FromBase64String(input);
                            int charCnt = decoder.GetCharCount(decData, 0, decData.Length);
                            char[] decodedChars = new char[charCnt];
                            decoder.GetChars(decData, 0, decData.Length, decodedChars, 0);
                            rez = new string(decodedChars);
                        }
                        catch (Exception e)
                        {
                            rez = "Exception in b64Base Decoding " + e.ToString();
                        }
                    }
                    int rezLen = rez.Length;
                    int inLen = input.Length;
                    return rez;
                }
                static string EncryptB64(byte[] input, bool bEncrypt)
                {
                    string rez = "";
                    if (bEncrypt)
                    {
                        try
                        {
                            rez = Convert.ToBase64String(input);
                        }
                        catch (Exception e)
                        {
                            rez = "Exception in b64Base Encoding " + e.ToString();
                        }
                    }
                    else
                    {
                        try
                        {
                            System.Text.UTF8Encoding encoder = new System.Text.UTF8Encoding();
                            System.Text.Decoder decoder = encoder.GetDecoder();

                            byte[] decData = input;
                            int charCnt = decoder.GetCharCount(decData, 0, decData.Length);
                            char[] decodedChars = new char[charCnt];
                            decoder.GetChars(decData, 0, decData.Length, decodedChars, 0);
                            rez = new string(decodedChars);
                        }
                        catch (Exception e)
                        {
                            rez = "Exception in b64Base Decoding " + e.ToString();
                        }
                    }
                    int rezLen = rez.Length;
                    int inLen = input.Length;
                    return rez;
                }

                public static byte[] GetBytesB64(string input)
                {
                    byte[] rez;
                    rez = Convert.FromBase64String(input);
                    return rez;
                }
                public static byte[] GetBytesB64Decode(string input)
                {

                    byte[] decData = Convert.FromBase64String(input);

                    System.Text.UTF8Encoding encoder = new System.Text.UTF8Encoding();
                    System.Text.Decoder decoder = encoder.GetDecoder();
                    int charCnt = decoder.GetCharCount(decData, 0, decData.Length);
                    char[] decodedChars = new char[charCnt];
                    decoder.GetChars(decData, 0, decData.Length, decodedChars, 0);
                    decData = Convert.FromBase64CharArray(decodedChars, 0, charCnt);

                    return decData;


                }

                public static string EncryptAES(string input, bool bEncrypt)
                {
                    string rez = input;
                    try
                    {
#if OS_W8
                        SymmetricKeyAlgorithmProvider aesAlg =
                            SymmetricKeyAlgorithmProvider.OpenAlgorithm(SymmetricAlgorithmNames.AesCbcPkcs7);

                        string key = GetKey();
                        byte[] decodedKeyBytes = GetBytesB64(key);

                        IBuffer keyMaterial = CryptographicBuffer.CreateFromByteArray(decodedKeyBytes);
                        CryptographicKey theKey = aesAlg.CreateSymmetricKey(keyMaterial);
                        byte[] decodedIVBytes = GetBytesB64(GetIV());
                        IBuffer iv = CryptographicBuffer.CreateFromByteArray(decodedIVBytes);

                        if (bEncrypt)
                        {
                            IBuffer encrypted;
                            IBuffer encrpyMe = CryptographicBuffer.CreateFromByteArray(Encoding.UTF8.GetBytes(input));

                            encrypted = Windows.Security.Cryptography.Core.CryptographicEngine.Encrypt(theKey, encrpyMe, iv);

                            rez = CryptographicBuffer.EncodeToBase64String(encrypted);

                        }
                        else
                        {
                            IBuffer decryptMe = CryptographicBuffer.CreateFromByteArray(GetBytesB64(input));
                            IBuffer decrypted;
                            decrypted = Windows.Security.Cryptography.Core.CryptographicEngine.Decrypt(theKey, decryptMe, iv);
                            byte[] decryptedBytes;
                            CryptographicBuffer.CopyToByteArray(decrypted, out decryptedBytes);
                            rez = Encoding.UTF8.GetString(decryptedBytes, 0, decryptedBytes.Length);
                        }
#else
                        AesManaged aesAlg = new AesManaged();
                        string key = GetKey();
                        byte[] decodedKeyBytes = GetBytesB64(key);
                        aesAlg.Key = decodedKeyBytes;
                        aesAlg.IV = GetBytesB64(GetIV());

                        if (bEncrypt)
                        {
                            byte[] encrypted;

                            ICryptoTransform encryptor = aesAlg.CreateEncryptor(aesAlg.Key, aesAlg.IV);

                            using (MemoryStream msEncrypt = new MemoryStream())
                            {
                                using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write))
                                {
                                    using (StreamWriter swEncrypt = new StreamWriter(csEncrypt))
                                    {
                                        swEncrypt.Write(input);
                                    }
                                    encrypted = msEncrypt.ToArray();
                                }
                            }
                            rez = EncryptB64(encrypted, true);
                        }
                        else
                        {
                            byte[] decryptMe = GetBytesB64(input);
                            ICryptoTransform encryptor = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV);

                            using (MemoryStream msDecrypt = new MemoryStream(decryptMe))
                            {
                                CryptoStream csDecrypt = new CryptoStream(msDecrypt, encryptor, CryptoStreamMode.Read);
                                using (StreamReader swDecrypt = new StreamReader(csDecrypt))
                                {
                                    rez = swDecrypt.ReadToEnd();
                                }
                            }
                        }
#endif
                    }
                    catch (Exception e)
                    {
                        string msg = "Encryptor error : " + e.ToString();
                        rez = msg;
                    }
                    return rez;
                }

                public static string Encrypt_MD5(string input)
                {
                    return MD5Core.GLMD5(System.Text.UTF8Encoding.UTF8.GetBytes(input));
                }
                // respect the multiple of 4 length needed for b64
                public static string PadString(string input)
                {
                    int len = input.Length % 4;
                    string rez = input;

                    if (len != 0)
                    {
                        rez = rez.PadRight(input.Length + 4 - len, '=');


                    }
                    return rez;
                }

                //AES key
                public static string GetKey()
                {

                    //Key returned in clear : GmA0FF9d004k9BPt167yNv==
                    byte[] aux = new byte[25];
                    byte[] vector = new byte[] {
                 0,     38,     212,    239,    22,     0,  243,    43,     204,    0,
                 4,     55,     206,    9,      14,     36, 189,    5,      1,      66,
                 213,   40,     199,    0,      0
            };
                    byte start = 71;

                    aux[0] = start;
                    for (int i = 1; i < 24; i++)
                    {
                        aux[i] = (byte)(aux[i - 1] + vector[i]);
                    }

                    return System.Text.Encoding.UTF8.GetString(aux, 0, 24);

                }
                //AES IV
                public static string GetIV()
                {
                    //IV returned in clear : OrdIpaV9jlrxPxdQRI0ud4==
                    byte[] aux = new byte[25];
                    byte[] vector = new byte[] {
                0,      35,     242,    229,    39,     241,    245,    227,    49,     2,
                6,      6,      216,     40,    236,    237,    1,      247,    231,    69,
                239,    208,    9,        0,    0
            };
                    byte start = 79;

                    aux[0] = start;
                    for (int i = 1; i < 24; i++)
                    {
                        aux[i] = (byte)(aux[i - 1] + vector[i]);

                    }

                    return System.Text.Encoding.UTF8.GetString(aux, 0, 24);

                }

            }
        }
    }
}
