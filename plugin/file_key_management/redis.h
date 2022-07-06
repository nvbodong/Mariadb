//
// Created by dongbo on 2022/7/5.
//

#ifndef MARIADB_10_9_1_REDIS_H
#define MARIADB_10_9_1_REDIS_H

#include <iostream>

#include <hiredis.h>
#include <hiredis_ssl.h>

#include "string_helper.h"
#include "shell.h"


const static std::string _ca_crt ="-----BEGIN CERTIFICATE-----\n"
                                  "MIIFWTCCA0GgAwIBAgIUM3rhsMpYXOFI0nIXhEgo5OS38H8wDQYJKoZIhvcNAQEL\n"
                                  "BQAwPDEaMBgGA1UECgwRbnZ4LWtleS1zZXJ2ZXItY2ExHjAcBgNVBAMMFUNlcnRp\n"
                                  "ZmljYXRlIEF1dGhvcml0eTAeFw0yMjA2MjgxMjI3NDJaFw0yMjA5MDYxMjI3NDJa\n"
                                  "MDwxGjAYBgNVBAoMEW52eC1rZXktc2VydmVyLWNhMR4wHAYDVQQDDBVDZXJ0aWZp\n"
                                  "Y2F0ZSBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQC9\n"
                                  "Kv7MJ/kUW1/kWGgChxSbwpXnrBhPRMdNt5MZZVFCSg7LUC8Y+4+0rHfg6AgkVbrg\n"
                                  "wb3ViHPz9/Ot6/4esZ6rZd81s0yA/ZpY7aXc3G22RNFvXYcA7EnuBUIaN9DNX7xG\n"
                                  "aMgHoKnLU8H0FtluGMc180rHhiMr/rakDh2Zs6UMFKrbWA3HbVkCgG/CMRwaEuYy\n"
                                  "xsvSyWtvj9slH2n7VPWYpvDvv3xfuVMPtO+w7wBJ3nJYKr62bwNOBjFv9+nXBBZb\n"
                                  "czTNTLsrx4+5ZUr8LeeMUeo10Y9oxiQqSQMh9A5nsRSzJ3kntRLgxYtWAJ9nCZGz\n"
                                  "65LPOQB5FRFqLkbSRhvmMuUJtt+eTXo8Hkp5kGCwigSWxi+wkKNb97mQ+zcxEAng\n"
                                  "YPLDPckPziQ4Cyd1OXO0qlaOfwyhCm5wQbJBMnu3pjGKUnHlk9K419/K64MdfunV\n"
                                  "1cRXmKqAeSbQjgw9BGMflqEC1z/m/1TxzspnHI1HdFJCdvrbmx/JLwIefJHw+9DZ\n"
                                  "Q7E15SZ6yJCSArbiSNfQ4Qhze+vMTJPSfYXKROofBsPVVx5OXdX6WqfwlUjndo3D\n"
                                  "a6Xzl1f4LdC4n7626poYg3+pUUmHakZer6AKyTOPoC3NLz59RHU/ZI8iobD9ypuV\n"
                                  "4ZcGN+rT6bfWrLnd+houzUtJqprrR4DCjz2GE1HGSQIDAQABo1MwUTAdBgNVHQ4E\n"
                                  "FgQUo/owJo8fN5A1sS8TWhOZiVuRrBEwHwYDVR0jBBgwFoAUo/owJo8fN5A1sS8T\n"
                                  "WhOZiVuRrBEwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAgEAjsLo\n"
                                  "T7oTQnK1K8q0UjxY83WoF1M/nrV5j1FcWvlXXeq2KIjJIL3dWcpink79Ibf3H+VM\n"
                                  "Gzhx/uCDWEv4mDUadCd+p3mxDKW18qIM07/NZw4YSCw1Frbzyi7nBjScEd7pxc2G\n"
                                  "r4cOUbzrSOV1c0rricrExHbW/dz88xR3I6YCli4+fBAyVFIDUvUI72ukkMxE13LC\n"
                                  "ukPvimwz3H7B/iH6QRx/id1C99pRnZ7ktLZBUX5j4zcjjMzsK4Wc7HAWbunW+aFd\n"
                                  "PdrYX1Y3pYs9RLabiEkjkYNKkw64BtwHrbeYsilAAhr7jFKgpUZ34P1ClzB0o3fb\n"
                                  "hgbxn7qaRM14SFXEQn0PZcdP5v1M8gPdp6E0Q15hWdlVD1BCjYnEMD1rrlB9ajAx\n"
                                  "xg2QVz+Kmi4tx4ObW1h6WVftd7WBq0E/O6Gi1ORp+cKHffxWo8zp50rNxKSv8VNC\n"
                                  "yolR62jUozr+OXDtBtrQQtfDjuNcl0L1tDID6lV878re7vJAbgh+E0n8+5vCJJ1I\n"
                                  "/O8P1DoUeLupmUvcYQBP1nLV/GcAFERv1NOEnRgHyGXHKpjo8XN2weuD7wHoz8Ek\n"
                                  "lKBa4P87wiwqqc2GO8+YmxuMurY6EyeqASHCXWTDwcjxwK4UNTL8D8LE6lkFq5es\n"
                                  "LZjIN994t94wdX+xhGj+OyIOQNp2HjnJZDx3ySk=\n"
                                  "-----END CERTIFICATE-----";

const static std::string _client_key ="-----BEGIN PRIVATE KEY-----\n"
                                      "MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQCtvJEvKlMF0iMR\n"
                                      "VbB6oldg12v/2Si+T/gRmZGoAHgVIy+eD6S2d7A2yMWliap1AiiuMsGT5VMh1AOo\n"
                                      "e2tcmmkIS1EjT7jiPLXSEDL91YozhWNjMbdHDCb28by7YEsFOE1sgAZWm2qDWYf6\n"
                                      "jxmloN5n+pF69aFYDpdUYp6ejxWYDnFv++cTYW0DPTD+7R38cpIIvJXVvVIi82lu\n"
                                      "q6+Imy0pJeYxNtqHuCTAOjFpsjtLp5Fst3Uaougi+HYls+or6EC0ZOlP14ewd/uA\n"
                                      "cmf5f6APntZVnn6y1ylykVNu2pepgIfcfAmZmTsG06TFQXfJvrpyN962Ed8Bvck0\n"
                                      "uJmAfnznAgMBAAECggEAFBO9/WLZPEt35PlvjwRyr4zFfMgGJtQL+VCzXeRL/DlY\n"
                                      "N95b7QAw22vhN0uM6L+qntLbvvEZ5cdf8tjsY8OuTpiRlFCSr4ZPhqp7kgzazzBj\n"
                                      "d2OX5/x2BkSSSF8M3FoQzHCRCvIj4XD3v1NJJfRMr1xr8ncvQYNSJpzM8srbNnqH\n"
                                      "1+r1i3lI/ppuElhKspiFl8gEKte5DkfoaB8l51B5zBzhDA/pHbu7bkrIILKm2Onv\n"
                                      "FZOYK0fzCsqLLhWYIlsZHNDx+2zvC2oNMubBsHiBV+TGiBgO6ngQMf4gNkcmYOUl\n"
                                      "IRWA8GIU4QLIhO3h6+fmrViFsSBclL4esj2gBDXsGQKBgQDn5aff1mCp5pcKvyZm\n"
                                      "Ng8VLxvY97TgdNO3J/mbdvjVtvWomhADbkJ8rITBjfvwKrFWhF4mISvP0NBuvhoO\n"
                                      "BPvqDU4unXJlrN78nxOQbj2zXQ41A0LkMPZofZD07ymPTATD/2KWgvVVUnMPHzS6\n"
                                      "aloqmZPwOOROuCaiXK3I+iWlHwKBgQC/y16+quHikYoFKh0dUfeEeqhCrXaOyJvl\n"
                                      "UPg3gvoA5JGVzIbQsGhq4KS9zngYm/3rjpCEEJlGXjzaGwexQkczFDMuOEVgd/Dt\n"
                                      "ip50U8PMZy6snbMZkYMZe5cnPFQjqKiRpJhB3AsrsEx0VFp9UymXZIYD9xVWYYhM\n"
                                      "O9T+MmsnOQKBgQCKb9tsETrbssDwtXRjCFqwttLVD+t3I8dQTBkC6l9alkLJSE/O\n"
                                      "/H/MleEa+3wvwAzmLVdXQ+43vdzwf6RmQitxakgGJBArCqyD+yNe0bEb5q2YkD3a\n"
                                      "pDA16OAkmLIizar6pvdgCwYSzYnKuUJtFxiE3fInMo95Pw4xV0qX9F2M2QKBgQCJ\n"
                                      "bQB7YFgwjxY1cyKfcq8HVxgfhslvr+C8pJLmu8vFPgHvMzjaaby38DU656l4ALYl\n"
                                      "xeZZKseY+0dhBIMziQkZWXz+vkU/xVF186BsO+UNHlX8S5ccV/53EO6Sp5CmaGA4\n"
                                      "05ZhgnjimhV+OT8Jk8A2gUnhp3a7iXEvJqQMbDYJSQKBgQDCd07NsqBmEOA2DsRq\n"
                                      "b4NOxjvSzc43yNPBQOGMgIw6WUImYQuEJa0Kb+q9TYsA5AxV5Te5lFvD42N3Cbv1\n"
                                      "SsomyY/OaDUcxFeXzmxPiAIQcdUrJWCK7vCvuZJ8g/vepoNS2j/uKphZOJ1MPwrZ\n"
                                      "CIl4S+GMUpEAq24izrMaJubE/w==\n"
                                      "-----END PRIVATE KEY-----";

const static std::string _client_crt ="-----BEGIN CERTIFICATE-----\n"
                                      "MIIEWzCCAkOgAwIBAgIUcfwpD7a2duLLUrj28PskYPl01WgwDQYJKoZIhvcNAQEL\n"
                                      "BQAwPDEaMBgGA1UECgwRbnZ4LWtleS1zZXJ2ZXItY2ExHjAcBgNVBAMMFUNlcnRp\n"
                                      "ZmljYXRlIEF1dGhvcml0eTAeFw0yMjA2MjgxMjI3NDJaFw0yMjA5MDYxMjI3NDJa\n"
                                      "MC8xFzAVBgNVBAoMDm52eC1rZXktc2VydmVyMRQwEgYDVQQDDAtDbGllbnQtb25s\n"
                                      "eTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAK28kS8qUwXSIxFVsHqi\n"
                                      "V2DXa//ZKL5P+BGZkagAeBUjL54PpLZ3sDbIxaWJqnUCKK4ywZPlUyHUA6h7a1ya\n"
                                      "aQhLUSNPuOI8tdIQMv3VijOFY2Mxt0cMJvbxvLtgSwU4TWyABlabaoNZh/qPGaWg\n"
                                      "3mf6kXr1oVgOl1Rinp6PFZgOcW/75xNhbQM9MP7tHfxykgi8ldW9UiLzaW6rr4ib\n"
                                      "LSkl5jE22oe4JMA6MWmyO0unkWy3dRqi6CL4diWz6ivoQLRk6U/Xh7B3+4ByZ/l/\n"
                                      "oA+e1lWefrLXKXKRU27al6mAh9x8CZmZOwbTpMVBd8m+unI33rYR3wG9yTS4mYB+\n"
                                      "fOcCAwEAAaNiMGAwCwYDVR0PBAQDAgWgMBEGCWCGSAGG+EIBAQQEAwIHgDAdBgNV\n"
                                      "HQ4EFgQUydJsoTXlDB5eKISCfFc8P8gqQkcwHwYDVR0jBBgwFoAUo/owJo8fN5A1\n"
                                      "sS8TWhOZiVuRrBEwDQYJKoZIhvcNAQELBQADggIBAI/j+AD2d98E3x75oZTNdPwY\n"
                                      "/p3OKD8hsUdU1cw+u46V4IvEOnpHfylb9F14y6TrUH69NqmIAdNZBlyw7sMB0cAM\n"
                                      "rpvGZEvelILSHMsooc+OXIpeHHhJ0E7GFA9Ja9nh70MPPvuZ59fdlyAbuCna960K\n"
                                      "GZ74bOSgapH2E+vuOajgddbM65omPCBteXJWnckOE1+vYIGyAhBBzgJ60lQJxrOc\n"
                                      "dyZEKHqCNiV9EkIBkPnLTIBgJEm1dZL6pJ1EmnSqs2Vx1KWCqU9GMkNwe6802Uag\n"
                                      "2HJkSUHN5NBXPtc9IQzidy4zaXckouBYDOaLSl2KE91CQH4U1tp24QpvmCMRQckg\n"
                                      "dw3nWIVbE9CvpOiuYmgakmFKBe8Pak0CDXNESczpkJ9fvYJ3WZSma1DoIHXvEdSm\n"
                                      "iZXe9t+gOu7t12OKe0y7EPYlC3Zv5qwSfBomSZkACy3fv/IaqOHn4765qEl2haig\n"
                                      "ZxROHC857yfB6HKksUg4uqZWPrikBs71YWy3zk0AoUDSeh5lmxn4dyX7LAxo0yBQ\n"
                                      "B6ikSarXhgXkbTAQ0ANvlgs0Qw59/LLKgyt6JPM8qctZoOyP27KvdbKGM8eCzuKt\n"
                                      "Bw2bcENju4jyyZbOvx7eCO9bIEQObDb9TjJYt+/G94pM9yLO9YsahfTB4fW4goCj\n"
                                      "mW2goc4q4wWcLiSB7MQ6\n"
                                      "-----END CERTIFICATE-----";



class kv_client {
public:
  kv_client(const std::string && key_server_ = "127.0.0.1",
        const std::string && key_server_port_ = "16379") :
                                                         key_server(key_server_),
                                                         key_server_port(key_server_port_) {
  }

  std::string system_call(const std::string &cmd);

  bool connect_NOSSL_kv_svr();

  bool connect_SSL_kv_svr();

  bool connect_kv_svr() ;

  bool  init_key_file();

  int rm_key_file();

  std::string decrypt(const std::string &crypto_str);

public:
  redisSSLContext *ssl;
  redisSSLContextError ssl_error;
  redisReply *reply;
  redisContext *redis_context = nullptr;

  const std::string cert = "/tmp/.redis_mariadb/client.crt";
  const std::string key = "/tmp/.redis_mariadb/client.key";
  const std::string ca =  "/tmp/.redis_mariadb/ca.crt";

  std::string key_server = "127.0.0.1";
  std::string key_server_port = "16379";
};

#endif // MARIADB_10_9_1_REDIS_H
