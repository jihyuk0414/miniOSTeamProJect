#!/bin/bash

for i in 1 2 3; do
    expect client_script.expect $i "안녕하세요 $i" &
done

wait

#병렬 테스트 위해서 사용했습니다.