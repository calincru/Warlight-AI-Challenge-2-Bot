#!/bin/bash

filename="AproapeBine.tar.gz"

create_archive() {
    mkdir -p archive
    cp -R ../include/* archive
    cp -R ../src/*.cpp archive
    cp -R ../src/*.h archive
    cp -R ../src/strategies/*.cpp archive
    cp -R ../src/strategies/*.h archive
    cp -R ../src/strategies/common archive
    tar czvf "$filename" -C ./archive .

    ret=$?
    rm -rf archive &>/dev/null
    return $ret
}

create_payload() {
    cat requestHeader "$filename" requestFooter > ~payload
    return $?
}

upload_payload() {
    if ! command -v curl >/dev/null; then
        echo "curl not present in PATH"
        return 1
    fi
    
    echo "CURL: logging in"
    curl --cookie-jar ~cookie "http://theaigames.com/sign-in" \
    -H "Host: theaigames.com" -H "User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:36.0) Gecko/20100101 Firefox/36.0" \
    -H "Accept: text/html, */*; q=0.01" \
    -H "Accept-Language: en-US,en;q=0.5" \
    --compressed -H "X-NewRelic-ID: VQIAWVFVGwEAUFBUAQI=" \
    -H "Content-Type: application/x-www-form-urlencoded; charset=UTF-8" \
    -H "X-PJAX: true" -H "X-PJAX-Container: #page" \
    -H "x-requested-with: XMLHttpRequest" \
    -H "Referer: http://theaigames.com/" \
    -H "DNT: 1" \
    -H "Connection: keep-alive" \
    -H "Pragma: no-cache" \
    -H "Cache-Control: no-cache" \
    --data "login=AproapeBine&password=050139899ecc3a268cb05a91d686958b&zAD2RZSTfdW2tH=bc1bfd0f3eea588d04df2e73cf3608ca" \
    &>/dev/null
    
    if [[ $? != 0 ]]; then
        return 1
    fi

    echo "CURL: uploading payload"
    curl --cookie ~cookie "http://theaigames.com/competitions/warlight-ai-challenge-2/bots/new" \
    -H "Host: theaigames.com" \
    -H "User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:36.0) Gecko/20100101 Firefox/36.0" \
    -H "Accept: */*" \
    -H "Accept-Language: en-US,en;q=0.5" \
    --compressed -H "X-NewRelic-ID: VQIAWVFVGwEAUFBUAQI=" \
    -H "x-requested-with: XMLHttpRequest" \
    -H "Referer: http://theaigames.com/profile" \
    -H "Content-Length: "$(wc -c ~payload | cut -d ' ' -f1) \
    -H "Content-Type: multipart/form-data; boundary=---------------------------190451771910450" \
    -H "DNT: 1" \
    -H "Connection: keep-alive" \
    -H "Pragma: no-cache" \
    -H "Cache-Control: no-cache" \
    --data-binary @~payload \
    &>/dev/null

    return $?
}

echo "Creating archive" && (create_archive || (echo "Could not create archive $filename" && false)) && \
echo "Creating payload" && (create_payload || (echo "Could not create payload" && false)) && \
echo "Uploading payload" && (upload_payload || (echo "CURL error" && false))
ret=$?
rm -f ~cookie ~payload
exit $?
