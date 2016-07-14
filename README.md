2015-2학기-시스템 프로그래밍
==========

demo02 : Static Library를 이용한 컴파일
----------
1) 목적 파일 만들기
  > gcc -c mytest.c foo.c boo.c bar.c

2) Static Library 만들기
> ar crv libmy.a foo.o boo.o bar.o

3) Static Library의 목차 만들기
> ranlib libmy.a

3) Static Library를 이용해 실행 파일 만들기
> gcc -o mytest mytest.o –L. -lmy 혹은
> gcc –o mytest mytest.o libmy.a

4) 심볼 확인 (어떤 함수가 포함돼있는지 확인)
> nm ./mytest

5) 이용한 라이브러리 확인 (Dynamic library)
> ldd ./mytest

----------

demo03 : 저수준과 고수준의 파일입출력 함수 비교
----------

200MB 정도 크기의 파일을 복사할 경우 저수준 함수를 이용했을 때와 고수준 함수를 이용했을 때 걸리는 시간을 비교한다. 버퍼의 크기가 작을 경우 고수준 방법이 빠른 반면에 버퍼의 크기가 일정 수준 이상으로 커질 경우에는 저수준 방법이 빠름을 확인할 수 있다.

----------

demo04 : Makefile 실습
----------

demo02에서 작성했던 소스를 Makefile을 이용하여 컴파일한다.

----------

demo05 : 프로세스 모니터링 프로그램
----------

test 라는 프로세스를 모니터링하는 testmon 프로그램을 작성한다. test는 2초에 한번씩 현재 시간을 터미널에 출력하는 프로그램이다. testmon은 test 프로세스 상태를 5초에 한번씩 터미널에 출력한다. 또한 testmon은 test 프로세스를 죽이거나, 재시작, 시작시킬 수 있다.
구현은 두 가지 버전으로 하였다.
1) 멀티 프로세스 방식. 즉 fork() 함수를 이용한 방법
2) 멀티 스레드 방식을 이용한 방법

----------

demo06 : 채팅 프로그램
----------

서버-클라이언트 모델의 one-to-one, many-to-many 채팅 프로그램을 구현하였다. many-to-many에서는 리눅스에서 제공하는 fd_set이라는 입출력 다중화 방식으로 여러 클라이언트의 동시 채팅을 지원하였다.

* Video Link : [Youtube][youtubelink]
[youtubelink]: https://youtu.be/ZHbp8yXfiPk

----------
