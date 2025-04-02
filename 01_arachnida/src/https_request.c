#include "spider.h"

void    init_open_ssl()
{
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

SSL_CTX    *seting_up_ssl_pointer()
{
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        fprintf(stderr, "Erreur SSL_CTX_new\n");
        return (NULL);
    }
    return (ctx);
}

BIO     *seting_up_bio_object(SSL **ssl, SSL_CTX *ctx)
{
    BIO *bio = BIO_new_ssl_connect(ctx);
    if (!bio)
    {
        fprintf(stderr, "Error: SSL connection failed\n");
        return (NULL);
    }
    BIO_get_ssl(bio, ssl);
    SSL_set_mode(*ssl, SSL_MODE_AUTO_RETRY);
    return (bio);
}

bool    open_secure_connection(BIO *bio, t_spider *data, SSL_CTX *ctx)
{
    char *host_port = strjoin(data->hostname, ":443");
    if (!host_port)
    {
        fprintf(stderr, "Error malloc, open_secure_connection\n");
        return (false);
    }

    // attempt connection
    BIO_set_conn_hostname(bio, host_port);
    free(host_port);
    // Verify the connection opened and perform the handshake
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_load_verify_locations(ctx, NULL, "/etc/ssl/certs");
    if (BIO_do_connect(bio) <= 0)
    {
        fprintf(stderr, "Error syscall, bio do connect\n");
        ERR_print_errors_fp(stderr);
        return (false);
    }
    return (true);
}

bool    check_valid_certificate(SSL *ssl, SSL_CTX *ctx, BIO *bio)
{
    if(SSL_get_verify_result(ssl) != X509_V_OK)
    {
        fprintf(stderr, "Error syscall, SSL_get_verfify_result\n");
        SSL_CTX_free(ctx);
        BIO_free_all(bio);
        return (false);
    }
    return (true);
}

bool    https_request(t_spider *data)
{
    SSL_CTX *ctx = NULL;
    BIO *bio = NULL;
    SSL *ssl = NULL;

    init_open_ssl();

    ctx = seting_up_ssl_pointer();
    if (!ctx)
        return (false);
    bio = seting_up_bio_object(&ssl, ctx);
    if (!bio)
    {
        SSL_CTX_free(ctx);
        return (false);
    }
    if (!open_secure_connection(bio, data, ctx))
    {
        SSL_CTX_free(ctx);
        BIO_free_all(bio);
        return(false);
    }
    if (!check_valid_certificate(ssl, ctx, bio))
    {
        SSL_CTX_free(ctx);
        BIO_free_all(bio);
        return(false);
    }
    return (true);
}