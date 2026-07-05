FROM espressif/idf:v4.4.8

ENV LANG=C.UTF-8 \
    LC_ALL=C.UTF-8

WORKDIR /workspace/firmware

RUN git config --system --add safe.directory /opt/esp/idf \
    && git config --system --add safe.directory /opt/esp/idf/components/openthread/openthread
