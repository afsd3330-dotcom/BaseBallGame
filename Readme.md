&#x20;1. 과제 소개

\- 멀티플레이어 환경에서 정답 숫자를 맞추는 야구 게임



2\. 구현 사항

\- GameMode 서버 로직: 플레이어 관리 및 게임 흐름 제어

\- 난수 생성: 중복 없는 3자리 숫자 생성

\- 판정 로직: 스트라이크, 볼, 아웃 계산

\- 상태 관리: 플레이어별 시도 횟수 관리

\- 리셋 기능: 게임 종료 후 자동 재시작

&#x20;GameMode 서버 로직 및 난수 생성

\- `ABBGGameModeBase::GenerateSecretNumber`: `TArray`와 `Swap` 알고리즘을 사용하여 1\~9 사이의 중복 없는 난수를 생성합니다.

\- `OnPostLogin`: 새로운 플레이어가 접속할 때마다 `PlayerState`를 통해 고유 이름을 부여하고 리스트에 관리합니다.



판정 및 예외 처리 로직

\- `JudgeResult`: 입력값과 정답을 비교하여 Strike, Ball, Out 개수를 계산.

시도 횟수 및 상태 관리

\- `ABBGPlayerState`: 플레이어별 `CurrentGuessCount`를 저장하여 최대 시도 횟수 제한 로직을 처리.

\- `ClientRPCPrintChatMessageString`: 서버에서 판정된 결과를 모든 클라이언트의 채팅창에 브로드캐스팅



게임 종료 및 자동 리셋

\- `JudgeGame`: 3 스트라이크 달성 시 승리자를 공표.

