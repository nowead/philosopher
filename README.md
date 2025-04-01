# 🧠 철학자들 (Philosophers) - 42서울

이 프로젝트는 **동시성(concurrency)** 개념을 배우기 위한 과제로, **Dining Philosophers Problem(식사하는 철학자 문제)** 를 시뮬레이션하는 프로그램입니다. C 언어에서 **스레드(pthread)** 와 **뮤텍스(mutex)** 를 이용한 **동기화(synchronization)** 를 중심으로 구성되어 있습니다.

---

## 📁 프로젝트 구조
```
philo/
├── philo/             # 필수 파트 (스레드 기반)
│   ├── Makefile
│   ├── death_checker.c
│   ├── main.c
│   ├── philo.c
│   ├── philo.h
│   ├── philo_utils.c
│   └── simulation.c
└── philo_bonus/       # 보너스 파트 (프로세스 + 세마포어 기반)
    ├── Makefile
    ├── death_checker_bonus.c
    ├── main_bonus.c
    ├── philo_bonus.c
    ├── philo_bonus.h
    ├── philo_utils_bonus.c
    └── simulation_bonus.c
```
---

## 🛠️ 컴파일 방법

### ✅ 필수 파트 (스레드 사용)
cd philo/philo
make

### ⭐ 보너스 파트 (프로세스 + 세마포어 사용)
cd philo/philo_bonus
make

---

## 🚀 실행 방법

./philo 철학자_수 죽는_시간 먹는_시간 자는_시간 [각 철학자가_식사해야_하는_횟수]

### 예시:
./philo 5 800 200 200

- 철학자 수: 5명
- 죽는 시간: 800ms
- 먹는 시간: 200ms
- 자는 시간: 200ms  
→ 이 조건으로 철학자 시뮬레이션이 실행됩니다.

> 마지막 인자인 `[각 철학자가_식사해야_하는_횟수]`는 선택사항입니다. 넣을 경우, 모든 철학자가 해당 횟수만큼 식사하면 프로그램이 종료됩니다.

---

## 📌 구현 조건 및 주의 사항

- 철학자는 **생각 → 포크 집기 → 식사 → 포크 내려놓기 → 잠자기** 사이클을 반복합니다.
- 철학자가 **죽는 조건**을 반드시 체크해야 합니다. (`time_to_die`보다 오래 굶으면 죽음)
- **데드락** 없이 프로그램이 동작해야 하며, **정확한 타이밍 관리**가 필수입니다.
- 보너스 파트에서는 다음이 추가됩니다:
  - **프로세스 기반 구현**
  - **세마포어를 통한 동기화**

---

## 🔧 주요 기술 요소

- pthread 스레드
- mutex를 통한 임계구역 보호
- semaphore (보너스)
- 정확한 시간 측정 및 sleep 처리
- 동기화 이슈 해결: 데드락, 경쟁 조건 방지

---

## 👨‍💻 만든 사람

민대원(Damin)

---
