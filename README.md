# 2020Embedded_contest_for_Autonomus_driving_FAFA

## FAFA means [Five assembled for Autonomous]

## 1. 프로젝트 소개
[![Video Label](/pic/표지.png)](https://youtu.be/E9MgEd5yksw)

카메라를 통한 영상인식으로 외부상황을 인지하고 센서로 주변 물체를 감지하며 자율주행하는 모형차의 임베디드 시스템을 개발한다.


## 2. Abstract

- Ethernet communications to the Linux operating system's Host PC and the TI TDA2x board provided.
To form the development environment for model cars. Using OpenCV to process information on lanes, traffic lights, signs, and stop lines with image recognition and infrared distance sensors to surrounding objects.
Calculating the values of the model car, which determines the external situation on its own and drives on its own.
Develop an algorithm.


## 3. 개발 목적 및 목표

3.1. 개발 목적

- 카메라를 통한 영상 인식과 적외선 거리 센서를 통해 수집한 정보를 토대로 차량이 주행하는데 있어서 외부 상황을 판단하여 차량을 스스로 제어하는 모형차를 제작하는 것이 목적이다. 

- 도로주행, 주차, 신호등, 교차로, 추월구간, 돌발 상황 등 10가지 미션 코스를 임베디드 환경에서 신속하고 정확하게 수행 할 수 있게 개발한다.

3.2. 개발 목표

- 자율주행은 실시간으로 입력받은 정보를 빠르게 처리하여 상황에 맞는 동작을 해야 하는 신속성이 요구된다. 
동시에 센서들로부터 값을 받아오고, 받아온 값들을 변수에 저장해 복잡한 동작을 구현하는 함수들이 서로 충돌하여 오류가 발생할 확률이 크다. 
이러한 점을 보완하고자 함수를 기능별로 모듈화 하여 관리하고 변수의 수를 최소화 한다. 
차량의 센서들로 부터 받아온 정보 중 필요한 정보만 선별하는 함수를 따로 구현하여 연산을 최대한 줄여 신속성을 높이는 것을 목표로 한다.


## 4. 향후 계획

- 영상 처리 기술과, 적외선 거리 센서를 이용한 자율주행 모형차를 개발한 경험을 토대로 향후 NVIDIA 플랫폼 임베디드 보드를 기반으로 자율주행 차량을 개발한다. 개발 중 발생할 수 있는 여러 상황을 빠른 연산속도로 최적화된 알고리즘 개발과 딥러닝을 통한 상황 예측/판단, Lidar 센서와 Radar 센서를 이용한 감지 기술로 해결하여 완전한 AI기반 자율주행 차량 개발을 목표로 한다.


### 개발 언어
- C++ 
- Python      

### 주요 라이브러리 
- opencv(3.4.0)   

  
<!--
## 4. 기타
-->
