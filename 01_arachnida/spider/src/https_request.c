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

BIO     *seting_up_bio_object(SSL **ssl, SSL_CTX *ctx, t_spider *data)
{
    BIO *bio = BIO_new_ssl_connect(ctx);
    if (!bio)
    {
        fprintf(stderr, "Error: SSL connection failed\n");
        return (NULL);
    }
    BIO_get_ssl(bio, ssl);
    SSL_set_mode(*ssl, SSL_MODE_AUTO_RETRY);
    SSL_set_tlsext_host_name(*ssl, data->hostname);
    return (bio);
}

bool    open_secure_connection(BIO *bio, t_spider *data, SSL_CTX *ctx)
{
    char *host_port = strjoin(data->hostname, ":443", false, false);
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
    if (SSL_get_verify_result(ssl) != X509_V_OK)
    {
        fprintf(stderr, "Error syscall, SSL_get_verfify_result\n");
        SSL_CTX_free(ctx);
        BIO_free_all(bio);
        return (false);
    }
    return (true);
}

bool    bio_write(t_spider *data, BIO *bio)
{
    char request[1024];

    snprintf(request, sizeof(request),
        "GET / HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "\r\n", data->hostname);
    if (BIO_write(bio, request, ft_strlen(request)) <= 0)
    {
        fprintf(stderr, "Error syscall, BIO write failed\n");
        return (false);
    }
    return (true);
}

bool    bio_write_to_url(t_spider *data, BIO *bio, char *url)
{
    char request[1024];

    snprintf(request, sizeof(request),
    "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
        "(KHTML, like Gecko) Chrome/90.0.0.0 Safari/537.36\r\n"
        "Referer: https://%s/\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n"
        "\r\n", url, data->hostname, data->hostname);

    if (BIO_write(bio, request, ft_strlen(request)) <= 0)
    {
        fprintf(stderr, "Error syscall, BIO write failed with %s\n", url);
        return (false);
    }
    return (true);
}

bool    bio_read(t_spider *data, BIO *bio)
{
    char buffer[16384];
    int bytes_reads;

    while ((bytes_reads = BIO_read(bio, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_reads] = 0;
        data->html_page = strjoin(data->html_page, buffer, true, false);
        if (!data->html_page)
        return (false);
    }
    if (data->html_page == NULL)
        return (false);
    
    if (!get_links(data))
        return (false);
    if (!find_images(data))
        return (false);
    return (true);
}

static bool    free_data(BIO *bio, SSL_CTX *ctx)
{
    SSL_CTX_free(ctx);
    BIO_free_all(bio);
    return (false);
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
    bio = seting_up_bio_object(&ssl, ctx, data);
    if (!bio)
        return (SSL_CTX_free(ctx), false);

    if (!open_secure_connection(bio, data, ctx))
        return (free_data(bio, ctx));

    if (!check_valid_certificate(ssl, ctx, bio))
        return (free_data(bio, ctx));

    if (!bio_write(data, bio))
        return (free_data(bio, ctx));
    if (!bio_read(data, bio))
        return (free_data(bio, ctx));
    if (strstr(data->html_page, "302 Moved Temporarily"))
        return (free_data(bio, ctx), err_msg("Redirection detected\n"));
    BIO_free_all(bio);
    bio = NULL;

    // tant que je n'ai pas lu assez de page je continue
    if (data->links_name_tab)
    {
        for (int i = 0; data->links_name_tab[i] && data->deepness > 0; i++)
        {
            bio = seting_up_bio_object(&ssl, ctx, data);
            if (!bio)
                return (SSL_CTX_free(ctx), false);
            if (!open_secure_connection(bio, data, ctx))
                return (free_data(bio, ctx));
            if (!check_valid_certificate(ssl, ctx, bio))
                return (free_data(bio, ctx));
            // read
            printf("requete sur %s\n", data->links_name_tab[i]);
            if (!bio_write_to_url(data, bio, data->links_name_tab[i]))
            {
                BIO_free_all(bio);
                continue;
            }
            if (!bio_read(data, bio))
                return (free_data(bio, ctx));
            BIO_free_all(bio);
            data->deepness--;
        }
        bio = NULL;
        for (int i = 0; data->links_name_tab[i]; i++)
            free(data->links_name_tab[i]);
        free(data->links_name_tab);
        data->links_name_tab = NULL;
    }
    else
        fprintf(stderr,"No links found in the index\n");

    // free links tab
    
    if (!data->img_name_tab)
        return (fprintf(stderr, "no .jpg/jpeg .png .gif .bmp found\n"), free_data(bio, ctx));
    // https download
    for (int i = 0; data->img_name_tab[i]; i++)
    {
        // get name
        if (!ft_openfile_in_dir(data, i))
            continue;
        if (!https_request_to_get_image(data, https_request_setup(bio, ssl, ctx, data), i))
            continue;
        // 
    }

    if (ctx)
        SSL_CTX_free(ctx);
    if (bio)
        BIO_free_all(bio);
    return (true);
}

BIO *https_request_setup(BIO *bio, SSL *ssl, SSL_CTX *ctx, t_spider *data)
{
    bio = seting_up_bio_object(&ssl, ctx, data);
    if (!bio)
        return (SSL_CTX_free(ctx), NULL);
    if (!open_secure_connection(bio, data, ctx))
        return (free_data(bio, ctx), NULL);
    if (!check_valid_certificate(ssl, ctx, bio))
        return (free_data(bio, ctx), NULL);
    return (bio);
}
