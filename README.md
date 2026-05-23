# 운명의 낙인 (Fated Brand)

**1학년 2학기 기말과제 1인 개발 프로젝트**

<br>

## 📋 Project Overview

| 항목 | 내용 |
|------|------|
| **장르** | 액션, 횡스크롤, 메트로배니아 |
| **개발 엔진** | Unreal Engine 5 |
| **언어** | C++, Blueprint |
| **플랫폼** | Windows (PC) |
| **개발 기간** | 2025-09-10 ~ 2025-12-15 (약 3개월) |
| **개발 인원** | 1인 |

<br>

## 📖 게임 스토리 & 컨셉

> *"낙인은 운명을 기록한다. 너는 빈 낙인이다."*

대죄를 저지른 주인공에게 심판자는 형벌로써 이 세계의 성운을 모두 회수하면 해방을 약속한다.
선택의 길이 없는 주인공은 성운을 회수하기 위해 모험을 떠난다.

플레이어는 탐험하며 각종 몬스터와 싸우기도, 때로는 함정에 빠지거나 장치에 의해 난관을 겪어가며 성장하는 과정을 담은 게임입니다.

**레퍼런스** : [Hollow Knight](https://www.hollowknight.com/) — 부적(참 = 스킬) 시스템, 월드 탐험 구조, 오브젝트 인터랙션을 분석해 스킬·스탯 시스템에 적용했습니다.

<br>

## 🎮 게임플레이 핵심 요소

| 요소 | 설명 |
|------|------|
| **성장** | 적과의 전투를 통해 플레이어가 성장하는 과정이 핵심. 적마다 능력치가 다르므로 성장이 필요합니다. |
| **강화** | 강화 재단(특정 위치)에서 EnhancedCore를 소비해 스탯을 업그레이드할 수 있습니다. |
| **수집** | 성운(EnhancedCore) 수집이 주 목표. 스킬 조합을 통해 플레이 자유도를 높입니다. |
| **세이브 포인트** | 구역 내 체크포인트에서 진행도를 저장합니다. |
| **휴식** | 휴식 구역에서 회복과 동시에 버프를 받을 수 있습니다. |

**게임 목표** : 맵을 탐험하며 성운 수집도를 높여 성장시키고 최종 보스를 물리치는 것

<br>

## 🗺️ 맵 구성

총 9개 맵으로 구성된 선형 스테이지 구조입니다.

```
메인 메뉴
  └─ 튜토리얼
       └─ 첫 번째 마을 (허브)
            ├─ 1구역 (FirstArea1)
            ├─ 2구역 (FirstArea2)
            ├─ 3구역 (FirstArea3)
            └─ 보스 통로
                 └─ 보스 스테이지
```

**레벨 디자인 컨셉** : 첫 스테이지의 배경은 플레이어가 가장 친숙하게 느낄 수 있는 **초원 지대**로 설정. 맵 구조는 **부유섬**을 키워드로 다양한 방식의 탐험이 가능하도록 디자인했습니다.

**레벨 구성 요소** : 세이브 포인트 · 스탯 강화 재단 · 휴식 구역 · 적과의 조우 · 함정 및 이동 플랫폼 · 성운 파편 · 로프 이동

<br>

## 🕹️ 키 조작

| 키 | 액션 | 기능 설명 |
|----|------|-----------|
| ← → | 좌우 이동 | 방향키에 맞는 방향으로 이동합니다. |
| ↑ | 점프 | 점프 상태가 되며, 세이브 포인트·강화 재단 등과의 상호작용에도 사용합니다. |
| ↑ ↑ | 더블 점프 | 점프 상태에서 한 번 더 점프 시 더블 점프가 발생합니다. (해금 필요) |
| ↑ + ← or → | 벽 점프 | 벽에서 점프한 이후 벽 방향으로 이동하는 상태에서 점프 시 벽 점프가 발생합니다. |
| ↓ | 체력 충전 | 일정 게이지(VitalSurge) 수치를 소모하여 체력을 충전합니다. |
| Space Bar | 기본 공격 | 플레이어의 기본 근접 공격이 발생하며, 적에게 피격 성공 시 스탯 기반 데미지를 줍니다. |
| Left Shift | 패링 | 특정 타이밍에 키를 누르면 패링이 발동되어 데미지를 무효화합니다. 성공 시 VitalSurge 게이지가 상승하며 특정치 도달 시 사용 가능해집니다. |
| 1 / 2 / 3 | 스킬 1 · 2 · 3 | 해당 슬롯에 대응하는 스킬이 발동됩니다. (패시브는 조건에 따라 자동 발동) |
| Q / W / E | 스킬 4 · 5 · 6 | 해당 슬롯에 대응하는 스킬이 발동됩니다. (패시브는 조건에 따라 자동 발동) |
| K | 스킬 설정 창 | 성운의 수집 현황을 보며 사용할 스킬을 구성하는 UI를 열고 닫습니다. |
| ESC | 일시 정지 | 게임을 일시 정지하며 메뉴 UI가 생성됩니다 (재개·다시 시작·메인 메뉴·게임 종료). |
| F1 | 키 설명 | 게임 내 사용하는 키를 안내하는 위젯을 키고 끕니다. |

<br>

## 🔄 게임플레이 루프

```
맵 진입 → 맵 탐색
              ├─ 이동 포탈 발견 → 목표 맵 이동
              ├─ 세이브 포인트 발견 → 원하는 슬롯에 세이브
              ├─ 강화 재단 발견 → 원하는 스탯 강화 → 스탯 반영 및 강화코어 소모
              └─ 함정 발견 → 피해 발생
                                  ├─ 체력 > 0 → 체력 감소, UI 반영
                                  └─ 체력 <= 0 → 플레이어 사망 연출 → 마지막 세이브 포인트로 롤백

전투 발생 → 전투 결과
              ├─ 패배 → 플레이어 사망 연출 → 마지막 세이브 포인트로 롤백
              └─ 승리 → 적 사망 연출 → 보상 지급 (EnhancedCore) · UI 반영
```

**리스크 / 보상 구조**
- **패링을 통한 게이지 충전** : 판정이 까다롭지만 성공 시 VitalSurge 게이지 충전으로 체력 회복 기회 획득
- **성운 획득** : 보스 처치뿐 아니라 맵 탐험 중에도 획득 가능, 탐험에 재미를 부여하는 장치
- **보스와의 전투** : 일반 적과 다른 형태이며, 처치 시 상응하는 보상 제공

<br>

## ⚙️ 핵심 시스템

### 1. GAS (Gameplay Ability System)

Unreal Engine의 GAS 프레임워크를 기반으로 전투 시스템 전체를 구현했습니다.

**어빌리티 목록**

| 분류 | 어빌리티 | 한국어명 | 설명 |
|------|---------|---------|------|
| 공격 (Offensive) | MeleeAttack | 근접 공격 | 기본 근접 콤보 |
| 공격 (Offensive) | BlastingZone | 블라스팅 존 | 범위 폭발 피해 |
| 공격 (Offensive) | NobleBlood | 노블 블러드 | 단일 적에게 강렬한 피해 |
| 공격 (Offensive) | Blocking | 방어 | 피해를 막는 능력 |
| 패시브 (Passive) | JugularRip | 저글라 립 | 특정 확률로 공격 시 치명타 발생 |
| 패시브 (Passive) | DoubleJump | 더블 점프 | 공중 2단 점프 해금 |
| 패시브 (Passive) | LivingDead | 리빙 데드 | 특정 조건으로 사망 시 부활, 300초간 재발동 없음 |
| 특수 | VitalSurge | 바이탈 서지 | 게이지를 소모해 체력 충전 |

**커스텀 실행 계산 (ExecCalc / MMC)**
- `ExecCalc_Damage` : 공격력 · 방어 여부 등을 종합해 최종 데미지를 계산
- `MMC_VitalSurge` : ModMagCalc를 통해 VitalSurge 게이지 보정치를 계산

<br>

### 2. 어트리뷰트 시스템 & 스탯 성장 곡선

`UFatedBrandAttributeSet`이 관리하는 캐릭터 스탯입니다.

| 어트리뷰트 | 설명 |
|-----------|------|
| **Strength** | 공격력에 영향을 주는 주 스탯 |
| **Vigor** | 최대 HP에 영향을 주는 주 스탯 |
| **CurrentHealth / MaxHealth** | 현재 & 최대 체력 |
| **AttackPower** | 실제 공격력 수치 (Strength 파생) |
| **VitalSurge** | 체력 충전 게이지 (0 ~ 100, 클램핑 처리) |
| **EnhancedCore** | 스탯 업그레이드 재화 |

**CurveTable 기반 스탯 성장 수치** (레벨 1 ~ 10)

| 스탯 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|------|---|---|---|---|---|---|---|---|---|---|
| Vigor / Strength | 0 | 100 | 200 | 400 | 800 | 1600 | 2400 | 3200 | 4000 | 5000 |
| MaxHealth | 60 | 70 | 80 | 90 | 100 | 110 | 120 | 130 | 140 | 150 |
| AttackPower | 1.0 | 1.1 | 1.2 | 1.3 | 1.4 | 1.5 | 1.6 | 1.7 | 1.8 | 2.0 |

<br>

### 3. 캐릭터 & 전투

```
AFatedBrandCharacterBase  (공통 전투 로직, ASC/AttributeSet 보유)
  ├─ AFatedBrandCharacter  (플레이어 — PlayerInterface 구현)
  └─ AFatedBrandEnemy      (적 — BehaviorTree + AIController)
```

**충돌 판정 구조**
- 무기(`StaticMeshComponent`)와 양손(`BoxComponent × 2`) 각각 독립적으로 히트박스 관리
- `EToggleDamageType`으로 어떤 충돌체를 활성화할지 어빌리티 단위로 제어
- `MotionWarpingComponent`로 공격 모션 중 위치·방향을 타겟에 맞게 보정

**적 AI**
- Behavior Tree + `AFatedBrandAIController`로 행동 패턴 구현
- 근접 공격 / 점프 공격 / 원거리 공격 A·B·C 총 5가지 패턴

<br>

### 4. 성운 메뉴 (Nebula Menu) — 스킬 시스템

K 키로 열리는, 어빌리티를 해금하고 퀵슬롯에 배치하는 전용 UI 시스템입니다.

- 총 **40개 소켓** 슬롯 구성 (`ENebulaSelectSocket`)
- 컨트롤러 방향키 / 축 입력으로 소켓 포커싱 및 확정
- 퀵슬롯(`1`·`2`·`3` / `Q`·`W`·`E`)과 연동, 장착 시 즉시 입력 태그 갱신
- `NebulaDescriptionTable` DataTable로 어빌리티 설명 텍스트 관리

<br>

### 5. 어트리뷰트 메뉴 — 스탯 강화 시스템

강화 재단(`AttributeUpgradePoint`) 인터랙션으로 진입하는 스탯 강화 시스템입니다.

- **EnhancedCore** 재화를 소비해 Strength / Vigor 수치를 직접 업그레이드
- `SpendEnhancedCoreTable` CurveTable로 업그레이드 비용 정의
- 변경 전 캐싱 → 확정·취소 구현 (`CachedSpendEnhancedCore`)
- 소모 재화 / 보유 재화가 UI에 실시간 반영됨

<br>

### 6. 세이브 / 로드 시스템

`UFatedBrandSaveGame`이 아래 데이터를 직렬화합니다.

```cpp
// 저장되는 주요 데이터
├─ 플레이어 위치 (PlayerStartTag — 체크포인트 태그)
├─ 스탯 (Vigor, Strength, CurrentHealth, VitalSurge, EnhancedCore)
├─ 어빌리티 상태 (SavedAbilities — 태그, 슬롯, 레벨, 쿨다운)
└─ 맵별 액터 상태 (SavedMaps → FSavedActor — 이름, 트랜스폼, 바이트 직렬화)
```

**Save 플로우** : 체크포인트에서 저장 선택 → `SaveInterface` & `PlayerInterface`로 `SaveProgress` 실행 → `GameModeBase`에서 데이터 수집 → `SaveGame`에 기록

**Load 플로우** : 게임 시작 또는 레벨 이동 후 슬롯 데이터 체크 (없으면 생성, 있으면 로드) → 저장 데이터 기반으로 플레이어 데이터 복원 → 월드 내 액터 Iterator 순회로 스폰 여부 결정

- `MVVM_LoadScreen` / `MVVM_LoadSlot` ViewModel 패턴으로 세이브 UI와 데이터 분리
- 멀티 슬롯(3개) 저장 지원, `ESaveSlotStatus`로 슬롯 유효성 구분
- `AFatedBrandPlayerState::CaptureRunningCooldowns` / `ApplyRunningCooldowns`로 레벨 이동 시 쿨다운 상태까지 보존

<br>

### 7. C++ / Blueprint 역할 분리

| 담당 | 역할 |
|------|------|
| **C++** | 재사용 가능한 모듈 제작, 입력 처리, 상태 플래그, Delegate 정의 등 도구·인프라 |
| **Blueprint** | 전투·연출 등의 실제 로직 조합, UI 바인딩 처리 |

C++에서는 제공(Provider)만 하고 실제 흐름은 Blueprint에서 조립되도록 해 역할 분리가 이루어졌습니다. 이를 통해 전투·연출 등의 로직을 Blueprint에서 빠르게 반복·수정할 수 있도록 했습니다.

**Blueprint Data 관리**
- `DataAsset` : 캐릭터·스킬·Input 등 정보 관리
- `CurveTable` : 플레이어 성장 곡선 관리
- `DataTable` : 게임 내 스크립트(어빌리티·어트리뷰트 설명) 관리

<br>

## 🏗️ 아키텍처

### 코어 클래스 관계

```
[Game Layer]
AFatedBrandGameModeBase ─── UFatedBrandInstance ─── UFatedBrandSaveGame
         │
AFatedBrandPlayerState (GAS Owner: ASC + AttributeSet + StatCurveTable 보유)

[Ability System]
UFatedBrandGameplayAbility ◁── UFatedBrandProjectileAbility
UFatedBrandAbilitySystemComponent ──▷ UFatedBrandAttributeSet
UExecCalc_Damage ───────────────────▷ UFatedBrandAttributeSet
UMMC_VitalSurge  ───────────────────▷ UFatedBrandAttributeSet

[Core]
AFatedBrandPlayerController ──▷ AFatedBrandPCM
AFatedBrandCharacterBase    ◁── AFatedBrandCharacter ─ - ▷ IPlayerInterface
                             └── AFatedBrandEnemy
AFatedBrandAIController ────────▷ AFatedBrandEnemy

[HUD & UI]
AFatedBrandHUD ──▷ UFatedBrandWidgetController
                        ├─ UOverlayWidgetController
                        ├─ UNebulaMenuWidgetController
                        ├─ UAttributeMenuWidgetController
                        └─ UPauseMenuWidgetController
ALoadScreenHUD ──▷ UMVVM_LoadScreen ──▷ UMVVM_LoadSlot (×3)
```

### 싱글톤 접근 허브 — UFatedBrandFunctionLibrary

- 프로젝트 전용 싱글톤 접근 허브로 보일러 플레이트를 제거
- 월드 컨텍스트가 Actor/Widget 등 어떤 타입이어도 내부에서 WorldContext로 정규화
- 전역 객체 타입을 프로젝트 고유 타입(FatedBrand)으로 고정해 일관된 타입 안정성 확보
- GameMode·HUD 등 월드 세팅 변경이 발생하더라도 접근 경로는 해당 라이브러리를 통해 타입 유지 가능

<br>

### 프로젝트 디렉토리 구조

```
Source/Project_FatedBrand/
├─ Public / Private
│   ├─ AbilitySystem/
│   │   ├─ Abilities/         — 기반 어빌리티 클래스, 투사체 어빌리티
│   │   ├─ AsyncTasks/        — WaitCooldownChange (쿨다운 비동기 감지)
│   │   ├─ ExecCalc/          — ExecCalc_Damage
│   │   ├─ ModMagCalc/        — MMC_VitalSurge
│   │   ├─ FatedBrandAbilitySystemComponent
│   │   └─ FatedBrandAttributeSet
│   ├─ Actors/
│   │   ├─ CheckPoint/        — 체크포인트, 어트리뷰트 업그레이드 포인트, 다음 목적지
│   │   ├─ FatedBrandEffectActor    — 트리거 기반 GE 적용 액터
│   │   ├─ FatedBrandInteractActor
│   │   ├─ FatedBrandProjectileActor
│   │   ├─ PlatformActor
│   │   └─ SpawnPoint
│   ├─ AnimInstance/          — 기반 + 플레이어 전용 AnimInstance
│   ├─ Characters/            — CharacterBase / Player / Enemy
│   ├─ Components/Input/      — FatedBrandEnhancedInputComponent
│   ├─ Controllers/           — PlayerController, AIController, PCM
│   ├─ DataAssets/            — InputConfig, AbilityInfo, StartUpDataBase
│   ├─ Game/                  — GameMode, GameInstance, PlayerState, SaveGame
│   ├─ HUD/
│   │   ├─ ViewModel/         — MVVM_LoadScreen, MVVM_LoadSlot
│   │   ├─ WidgetController/  — Overlay, Nebula, Attribute, Pause, 기반
│   │   └─ Widgets/           — FatedBrandUserWidget, LoadScreenWidget
│   └─ Interfaces/            — Combat, Player, Save, ActorInteract
│
├─ FatedBrandGameplayTags     — Native Gameplay Tag 중앙 관리
├─ FatedBrandEnumTypes        — 프로젝트 전역 열거형
├─ FatedBrandStructTypes      — 전역 구조체 (DamageEffectParams 등)
└─ FatedBrandFunctionLibrary  — 공통 유틸 함수 (싱글톤 접근 허브)
```

<br>

## 🔧 사용 플러그인 / 모듈

| 모듈 | 용도 |
|------|------|
| `GameplayAbilities` | GAS 전투 프레임워크 |
| `GameplayTags` | 태그 기반 상태 · 능력 식별 |
| `GameplayTasks` | GAS 내부 태스크 처리 |
| `EnhancedInput` | 입력 바인딩 및 입력 태그 매핑 |
| `AIModule` | Behavior Tree 기반 적 AI |
| `Niagara` | VFX (파티클 이펙트) |
| `MotionWarping` | 공격 모션 워프 (타겟 위치 보정) |

<br>

## 📅 개발 일정

| 기간 | 작업 내용 |
|------|-----------|
| **9월** | 플레이어 및 적 베이스 설계, Gameplay Ability System 설계 |
| **10월** | 인터랙션 액터 설계, 세이브 & 로드 시스템 설계 |
| **11월** | 레벨 디자인 및 테스트, 이펙트 & 사운드 적용, 1차 패키징 테스트 |
| **12월** | 패키징 시 발생 버그·이슈 클로징, 최종 패키징 |

<br>

## 🐛 주요 문제 해결 사례

**Issue #1 — Shipping Packaging 후 LoadScreen에서 게임 로드 시 Fatal Error 발생**

- 원인 : Shipping Packaging으로 SaveData가 없는 경우 Default Name으로 출력되어 Shipping에서 SaveData가 어디에도 존재하지 않았음
- 해결 : `%LOCALAPPDATA%` 경로의 SaveGames 데이터 제거 후 로직 재구성

**Issue #2 — 레벨 이동 후 GA 쿨다운이 초기화되는 문제**

- 원인 : GAS Cooldown 구현 시 `SetByCaller`를 통한 값 변경으로 적용했으나, 스킬 발동 시 `SetByCaller`가 불러올 값이 없어 쿨다운이 미적용
- 해결 : `ApplyCooldown` 메서드를 Override하여 Super를 생략하고 커스텀 로직으로 동작하도록 변경. GA Blueprint 생성 시 Cooldown Value 프로퍼티를 노출시켜 쿨다운이 돌아가지 않을 때 (0일 때) 프로퍼티 값으로 적용될 수 있도록 처리

> 추후 동일 현상 재발에 대비해 GitHub Issues에 발생 문제·원인 분석·해결 방안을 상세 기록해 추적이 원활하도록 관리했습니다.

<br>

## 🗒️ 개발 회고

**잘 된 점**
- GAS의 ExecCalc · MMC · GameplayCue 파이프라인을 직접 구현해 데미지 계산 흐름을 명확히 분리했습니다.
- MVVM 패턴 적용으로 세이브 UI의 로직과 뷰를 분리해 유지보수성을 높였습니다.
- C++ / Blueprint 역할 분리로 전투·연출 로직을 Blueprint에서 빠르게 반복·수정할 수 있었습니다.
- GitHub Issues로 버그 발생·원인·해결 과정을 문서화해 추적성을 확보했습니다.

**아쉬운 점**
- 1인 개발 특성상 에셋 제작에 시간을 충분히 쓰지 못했습니다.
- 멀티플레이어 Replication은 구현 범위 밖이었습니다.

**배운 점**
- GAS의 어트리뷰트 태그 매핑(`TagsToAttributes`), 쿨다운 비동기 감지(`WaitCooldownChange`), 맵별 액터 직렬화까지 UE5 심화 기능을 한 프로젝트 안에서 통합하는 경험을 쌓았습니다.
- CurveTable 기반 스탯 성장 곡선 설계를 통해 데이터 주도 게임 밸런싱의 기초를 익혔습니다.
