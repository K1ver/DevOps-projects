FROM nginx:latest

COPY hello.c /home
COPY nginx.conf /etc/nginx

RUN apt-get update && apt-get install -y gcc libfcgi-dev spawn-fcgi nginx && rm -rf /var/lib/apt/lists \
    && chmod u-s usr/bin/newgrp \
    && chmod u-s usr/bin/su \
    && chmod u-s usr/bin/mount \
    && chmod u-s usr/bin/chsh \
    && chmod u-s usr/bin/passwd \
    && chmod u-s usr/bin/umount \
    && chmod u-s usr/bin/gpasswd \
    && chmod u-s usr/bin/chfn \
    && chmod g-s usr/bin/expiry \
    && chmod g-s usr/sbin/unix_chkpwd \
    && chmod g-s usr/bin/chage
RUN gcc /home/hello.c -o /home/hello -lfcgi

RUN useradd -d /home/darrelle -m -s /bin/bash darrelle
USER darrelle

HEALTHCHECK --interval=5m --timeout=3s \
CMD curl -f http://localhost/ || exit 1

CMD spawn-fcgi -p 8080 /home/hello && nginx -g "daemon off;"