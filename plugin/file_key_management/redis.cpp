//
// Created by dongbo on 2022/7/5.
//

#include "redis.h"
std::string crt_key_path = "/tmp/.redis_mariadb";


std::string kv_client:: system_call(const std::string &cmd) {
  return paddle::framework::shell_get_command_output(cmd, 30, 2);
}


bool kv_client::init_key_file()
{

  if(!PathExists(paddle::string::format_string("%s/", crt_key_path.c_str()))){
      MkDir(paddle::string::format_string("%s/", crt_key_path.c_str()).c_str());
  }

  std::string r;
  {
    auto &&cmd = paddle::string::format_string("echo \"%s\" > %s/ca.crt",_ca_crt.c_str(), crt_key_path.c_str());
    ExecShellCommand(cmd, &r);
  }

  {
    auto &&cmd = paddle::string::format_string("echo \"%s\" > %s/client.crt",_client_crt.c_str(), crt_key_path.c_str());
    ExecShellCommand(cmd, &r);
  }

  {
    auto &&cmd = paddle::string::format_string("echo \"%s\" > %s/client.key",_client_key.c_str(), crt_key_path.c_str());
    ExecShellCommand(cmd, &r);
  }

  return true;
}

std::string kv_client::decrypt(const std::string &crypto_str)
{
  assert(crypto_str.length()> 0);
  auto conn_flag = connect_kv_svr();
  assert(conn_flag == true);

  redisReply *redis_reply= nullptr;
  redisAppendCommand(redis_context, "rsa_decrypt %b %s", crypto_str.c_str(),
                     crypto_str.length(), "");
  redisGetReply(redis_context, reinterpret_cast<void **>(&redis_reply));


  auto r = std::string(redis_reply->str, redis_reply->len);

  freeReplyObject(redis_reply);
  redisFree(redis_context);

  return r;
}

bool kv_client::connect_NOSSL_kv_svr(){
  struct timeval timeout = {10, 0};
  redis_context =
      reinterpret_cast<redisContext *>(redisConnectWithTimeout(
          key_server.c_str(), std::atoi(key_server_port.c_str()), timeout));

  if (redis_context == nullptr) {
    std::cout << "no connection established." << std::endl;
    return false;
  } else if (redis_context->err) {
    std::cout << "connection encounts errors: " << redis_context->errstr << std::endl;
    return false;
  }
  return true;
}

bool kv_client::connect_SSL_kv_svr(){
  init_key_file();

  redisInitOpenSSL();
  ssl = redisCreateSSLContext(
      ca.c_str(),
      NULL,
      cert.c_str(),
      key.c_str(),
      NULL, &ssl_error);

  if (!ssl) {
    printf("SSL Context error: %s\n",
           redisSSLContextGetError(ssl_error));
    return false;
  }

  struct timeval tv = { 10, 0 };
  redisOptions options{0};

  REDIS_OPTIONS_SET_TCP(&options, key_server.c_str(), std::atoi(key_server_port.c_str()));
  options.connect_timeout = &tv;

  while (redis_context == NULL || redis_context->err) {
    redis_context = redisConnectWithOptions(&options);
  }

  if (redis_context == NULL || redis_context->err) {
    if (redis_context) {
      printf("Connection error: %s\n", redis_context->errstr);
      redisFree(redis_context);
    } else {
      printf("Connection error: can't allocate NVX-KeyServer context\n");
    }
    return false;
  }

  if (redisInitiateSSLWithContext(redis_context, ssl) != REDIS_OK) {
    printf("Couldn't initialize SSL!\n");
    printf("Error: %s\n", redis_context->errstr);
    redisFree(redis_context);
    return false;
  }
  rm_key_file();
  return true;
}

bool kv_client::connect_kv_svr() {
  return connect_SSL_kv_svr();
}


int  kv_client::rm_key_file(){
  std::string r;
  ExecShellCommand(paddle::string::format_string("rm -rf %s/*", crt_key_path.c_str()),&r);
  return 0;
}
