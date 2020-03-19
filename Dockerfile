FROM jashook/vs2017-build-tools

RUN echo "hello world"

RUN choco install opencv

RUN dir c:/tools