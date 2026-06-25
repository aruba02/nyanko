# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <Windows.h>

#define MAX_UNIT 80
#pragma comment(lib, "winmm.lib")

typedef struct {
    int hp;
    int atk;
    int x;
    int y;
    char shape;
    int alive;
} cat;

typedef struct {
    int Text_Power;
    int moneySpeed;
    int stage;
    int cat1;
    int cat2;
    int cat3;
}py;

HANDLE handle;
CONSOLE_SCREEN_BUFFER_INFO prev;
char player[101];
int Map[30][120];
py playData;
int selectedStage;

void Loading(); // 로딤화면을 띄우는 함수
void Setting(); // 설정을 함
void Login(); // 로그인 및 회원가입 화면을 담당하는 함수
void PlayData(); // 각종 자원 등의 데이터를 관리하는 함수
void Upgrade(); // 업그레이드 화면을 담당하는 함수
void StageSelect(); // 말그대로 스테이지 고르는 함수
void LoadMain(); // 메인화면 보여주는 함수
void InGame(); // 인게임 - 즉 성부수기 게임 진행하는 함수
void mapEdge(); // Map에 테두리를 넣어주는 함수
void PrintMap(); // Map을 출력하는 함수
void printTextCenter(char text[]); // 텍스트를 화면의 정중앙에 출력하는 함수
void WhereText(int y, int x, const char text[]); // Map 배열의 원하는 곳에 문자열을 넣는 함수
void Gethandle() { handle = GetStdHandle(STD_OUTPUT_HANDLE); } // handle 받는 애
void SoundPlay(int); // 소리 재생하는 애


// 게임에 모든 함수 호출을 담당
void START() {
    Setting();
    Loading();
    Login();
    PlayData();
    system("cls");
    Loading();
    char text[1000] = "환영합니다, ";
    strcat(text, player); strcat(text, "님");
    printTextCenter(text);
    Sleep(1000);

    LoadMain();
    Sleep(1000);
}

int main() {
    return START(), 0;
}


void Setting() {
    CONSOLE_CURSOR_INFO CI;
    Gethandle();

    CI.dwSize = 1;
    CI.bVisible = FALSE;

    SetConsoleCursorInfo(handle, &CI);
}

void Login() {
    FILE* fp = fopen("users.txt", "a+");

Choose:
    system("cls");
    char text[1000] = "로그인 하려면 0, 회원가입 하려면 1을 입력하세요.";
    printTextCenter(text);
    printf("\t \t \t입력: ");
    int a = -1;
    scanf("%d", &a);
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    if (a != 0 && a != 1) {
        system("cls");
        char text[1000] = "잘못된 입력입니다. 다시 입력해주세요.";
        printTextCenter(text);
        Sleep(1000);
        goto Choose;
    }
    while ((ch = getchar()) != '\n' && ch != EOF);
    if (a == 0) {
        char ID[1001], PW[1001], rei[1001], rep[1001];
        int attempts = 3;
        while (attempts--) {
            int flag = 0;
            system("cls");
            char text1[1000] = "아이디를 입력하세요.";
            printTextCenter(text1);
            printf("\t \t \t입력: ");
            scanf("%1000s", ID);
            system("cls");
            char text2[1000] = "비밀번호를 입력하세요.";
            printTextCenter(text2);
            printf("\t \t \t입력: ");
            scanf("%1000s", PW);
            fseek(fp, 0, SEEK_SET);
            while (fscanf(fp, "%1000s %1000s", rei, rep) == 2) {
                if (strcmp(ID, rei) == 0 && strcmp(PW, rep) == 0) {
                    flag = 1;
                    break;
                }
            }
            system("cls");

            if (flag) {
                strcpy(player, ID);

                char text3[1000] = "로그인 성공";
                printTextCenter(text3);

                Sleep(1000);
                fclose(fp);
                return;
            }
            else {
                char text4[1000] = "아이디 혹은 비밀번호가 잘못되었습니다.";
                printTextCenter(text4);

                Sleep(1000);
            }
        }

        goto Choose;
    }
    else if (a == 1) {
        char nid[1001], pw[1001];
        int attempts = 3;
        while (attempts--) {
            system("cls");
            char text1[1000] = "회원가입할 아이디를 입력하세요(글자수 제한: 100)";
            printTextCenter(text1);
            printf("\t \t \t입력: ");
            scanf("%1000s", nid);

            if (strcmp(nid, "admin") == 0) {
                system("cls");
                char text2[1000] = "admin은 사용할 수 없는 아이디입니다.";
                printTextCenter(text2);
                Sleep(1000);
                continue;
            }
            if (strlen(nid) > 100) {
                system("cls");
                char text3[1000] = "아이디는 100자 이하로 입력해주세요.";
                printTextCenter(text3);
                Sleep(1000);
                continue;
            }
            fseek(fp, 0, SEEK_SET);
            int flag = 0;
            char ri[1001], rp[1001];
            while (fscanf(fp, "%1000s %1000s", ri, rp) == 2) {
                if (strcmp(nid, ri) == 0) {
                    system("cls");

                    char text4[1000] = "이미 존재하는 아이디입니다.";
                    printTextCenter(text4);

                    Sleep(1000);
                    flag = 1;
                    break;
                }
            }
            if (flag) {
                continue;
            }
            system("cls");
            char text5[1000] = "비밀번호를 설정하세요.";
            printTextCenter(text5);
            printf("\t \t \t입력: ");
            scanf("%1000s", pw);
            fseek(fp, 0, SEEK_END);
            fprintf(fp, "%s %s\n", nid, pw);
            strcpy(player, nid);
            system("cls");
            char text6[1000] = "회원가입 성공";
            printTextCenter(text6);
            Sleep(1000);
            fclose(fp);
            return;
        }
        goto Choose;
    }
    else {
        system("cls");
        char text1[1000] = "잘못된 입력입니다. 다시 입력해주세요.";
        printTextCenter(text1);
        Sleep(1000);
        goto Choose;
    }
}

void PlayData() {
    static int loaded = 0;
    FILE* fp;
    if (!loaded) {
        fp = fopen(player, "r");
        if (fp == NULL) {
            playData.Text_Power = 1;
            playData.moneySpeed = 6;
            playData.stage = 1;
            playData.cat1 = 0;
            playData.cat2 = 0;
            playData.cat3 = 0;
        }
        else {
            fscanf(fp, "%d %d %d %d %d %d",
                &playData.Text_Power, &playData.moneySpeed, &playData.stage, &playData.cat1, &playData.cat2, &playData.cat3);

            fclose(fp);
        }
        loaded = 1;
    }
    fp = fopen(player, "w");
    if (fp == NULL) {
        return;
    }
    fprintf(fp, "%d %d %d %d %d %d\n",
        playData.Text_Power, playData.moneySpeed, playData.stage, playData.cat1, playData.cat2, playData.cat3);
    fclose(fp);
}

void mapEdge() {
    memset(Map, ' ', sizeof(Map));
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 120; j++) {
            if (i == 0 && j == 0) Map[i][j] = '┏';
            else if (i == 0 && j == 119) Map[i][j] = '┓';
            else if (i == 29 && j == 0) Map[i][j] = '┗';
            else if (i == 29 && j == 119) Map[i][j] = '┛';
            else if (i == 0 || i == 29) Map[i][j] = '━';
            else if (j == 0 || j == 119) Map[i][j] = '┃';
        }
    }
}

void LoadMain_1() {
    SoundPlay(1);
    mapEdge();
    WhereText(1, 0, " ======================================================================================================================");
    WhereText(2, 0, " ||                                                                                                                  ||");
    WhereText(3, 0, " ||#####  #####  #####  #####  #####  #####  #####  #####  #####  #####  #####  #####  ##############################||");
    WhereText(4, 0, " ||#   #  #   #  #   #  #   #  #   #  #   #  #   #  #   #  #   #  #   #  #   #  #   #  ##           ^_^            ##||");
    WhereText(5, 0, " ||#####  #####  #####  #####  #####  #####  #####  #####  #####  #####  #####  #####  ########            ##########||");
    WhereText(6, 0, " ||                                                                                                                  ||");
    WhereText(7, 0, " ||       TTTTT  EEEEE  U   U  K   K  M   M  U   U  N   N      DDDD   AAAAA  EEEEE  JJJJJ  EEEEE  OOO   N   N        ||");
    WhereText(8, 0, " ||         T    E      U   U  K  K   MM MM  U   U  NN  N      D   D  A   A  E        J    E     O   O  NN  N        ||");
    WhereText(9, 0, " ||         T    EEEE   U   U  KKK    M M M  U   U  N N N      D   D  AAAAA  EEEE     J    EEEE  O   O  N N N        ||");
    WhereText(10, 0, " ||         T    E      U   U  K  K   M   M  U   U  N  NN      D   D  A   A  E     J  J    E     O   O  N  NN        ||");
    WhereText(11, 0, " ||         T    EEEEE   UUU   K   K  M   M   UUU   N   N      DDDD   A   A  EEEEE  JJ     EEEEE  OOO   N   N        ||");
    WhereText(12, 0, " ||                                                                                                                  ||");
    WhereText(13, 0, " ||                                                 <<<  특문 대전  >>>                                              ||");
    WhereText(14, 0, " ||                                                   <따따따라따다>                                                 ||");
    WhereText(15, 0, " ||                                                                                                                  ||");
    WhereText(16, 0, " ||                                                                                                                  ||");
    WhereText(17, 0, " ||                          +--------------------------+      +--------------------------+                          ||");
    WhereText(18, 0, " ||                          |          START           |      |           GROW           |                          ||");
    WhereText(19, 0, " ||                          +--------------------------+      +--------------------------+                          ||");
    WhereText(20, 0, " ||                                                                                                                  ||");
    WhereText(21, 0, " ||                              PRESS 1 : START                         PRESS 2 : GROW                              ||");
    WhereText(22, 0, " ||                                                                                                                  ||");
    WhereText(23, 0, " ||                            o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o=o                             ||");
    WhereText(24, 0, " ||                    ( o.o )        <#> <$ > <%> <&> <@>     SPECIAL LETTERS READY     ( -.- )                     ||");
    WhereText(25, 0, " ||                       > ^ <             -=-=-=-=-=     VS     =-=-=-=-=-             > ^ <                       ||");
    WhereText(26, 0, " ||                                                                                                                  ||");
    WhereText(27, 0, " ||#####  #####  #####  #####  #####  #####  #####  #####  #####  #####  #####  #####                                ||");
    WhereText(28, 0, " ======================================================================================================================");
}

void LoadMain() {
    LoadMain_1();
    while (1) {
        PrintMap();
        if ((GetAsyncKeyState('1') & 0x8000) || (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)) {
            Sleep(200);
            StageSelect();
            system("cls");
            LoadMain_1();
        }
        else if ((GetAsyncKeyState('2') & 0x8000) || (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)) {
            Sleep(200);
            Upgrade();
            system("cls");
            LoadMain_1();
        }
        Sleep(100);
    }
}

void StageSelect() {
    while (1) {
        system("cls");
        mapEdge();

        char text[200];
        WhereText(3, 4, "============================== STAGE SELECT ==============================");
        snprintf(text, sizeof(text), "현재 열린 스테이지 : %d / 3", playData.stage);
        WhereText(5, 4, text);
        WhereText(8, 4, "1번 : STAGE 1  기본 전투");
        WhereText(9, 4, "2번 : STAGE 2  빠른 적 등장");
        WhereText(10, 4, "3번 : STAGE 3  강한 적 군단!");
        WhereText(12, 4, "0번 : 메인으로 돌아가기");
        WhereText(15, 4, "이전 스테이지를 클리어해야 다음 스테이지를 선택할 수 있습니다.");
        WhereText(17, 4, "입력 : ");
        PrintMap();

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(handle, &csbi);
        int conwidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int conheight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        int startx = (conwidth - 120) / 2;
        int starty = (conheight - 30) / 2;
        if (startx < 0) startx = 0;
        if (starty < 0) starty = 0;
        COORD cursorPos;
        cursorPos.X = startx + 11;
        cursorPos.Y = starty + 17;
        SetConsoleCursorPosition(handle, cursorPos);

        int n;
        scanf("%d", &n);

        if (n == 0) return;

        if (n >= 1 && n <= 3) {
            if (n <= playData.stage) {
                selectedStage = n;
                SoundPlay(5);
                Sleep(3000);
                InGame();
                return;
            }
            else {
                WhereText(20, 4, "아직 잠긴 스테이지입니다. 이전 스테이지를 먼저 클리어하세요.");
                PrintMap();
                Sleep(1000);
            }
        }
        else {
            WhereText(20, 4, "입력이 잘못되었습니다.");
            PrintMap();
            Sleep(700);
        }
    }
}

void LoadCastle() {
    int chk = 0;
    Map[23][0] = '┣';
    Map[7][0] = '┣';
    Map[23][119] = '┫';
    Map[7][119] = '┫';
    for (int i = 1; i < 119; i++) {
        if (i == 25 || i == 93) {
            Map[23][i] = '┻';
            continue;
        }
        if (i < 25 || i > 93) {
            if (i == 6 || i == 11 || i == 16 || i == 21 || i == 113 || i == 108 || i == 103 || i == 98) {
                if (chk) {
                    Map[7][i] = '┏';
                    Map[23][i] = '━';
                    chk = 0;
                    continue;
                }
                Map[7][i] = '┓';
                Map[23][i] = '━';
                chk = 1;
                continue;
            }
            if (chk) {
                Map[9][i] = '━';
                Map[23][i] = '━';
                continue;
            }
            Map[7][i] = '━';
        }
        Map[23][i] = '━';
    }
    for (int i = 7; i <= 22; i++) {
        if (i == 8) {
            Map[i][6] = '┃';
            Map[i][11] = '┃';
            Map[i][16] = '┃';
            Map[i][21] = '┃';
            Map[i][113] = '┃';
            Map[i][103] = '┃';
            Map[i][108] = '┃';
            Map[i][98] = '┃';
        }
        // ┗ ┛
        if (i == 9) {
            Map[i][6] = '┗';
            Map[i][11] = '┛';
            Map[i][16] = '┗';
            Map[i][21] = '┛';
            Map[i][113] = '┛';
            Map[i][103] = '┛';
            Map[i][108] = '┗';
            Map[i][98] = '┗';
        }
        Map[i][25] = '┃';
        Map[i][93] = '┃';
    }
    Map[7][25] = '┓';
    Map[7][93] = '┏';
}

void PrintMap() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(handle, &csbi);
    if (prev.srWindow.Left != csbi.srWindow.Left ||
        prev.srWindow.Right != csbi.srWindow.Right ||
        prev.srWindow.Top != csbi.srWindow.Top ||
        prev.srWindow.Bottom != csbi.srWindow.Bottom) {
        system("cls");
    }
    prev = csbi;
    int conwidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int conheight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int startx = (conwidth - 120) / 2;
    int starty = (conheight - 30) / 2;
    if (startx < 0) startx = 0;
    if (starty < 0) starty = 0;
    COORD cursorPos;
    cursorPos.X = startx;
    cursorPos.Y = starty;
    for (int i = 0; i < 30; i++) {
        SetConsoleCursorPosition(handle, cursorPos);
        for (int j = 0; j < 120; j++) {
            if (Map[i][j] == '┃') printf("┃");
            else if (Map[i][j] == '━') printf("━");
            else if (Map[i][j] == '┗') printf("┗");
            else if (Map[i][j] == '┛') printf("┛");
            else if (Map[i][j] == '┏') printf("┏");
            else if (Map[i][j] == '┓') printf("┓");
            else if (Map[i][j] == '┣') printf("┣");
            else if (Map[i][j] == '┫') printf("┫");
            else if (Map[i][j] == '┻') printf("┻");
            else printf("%c", Map[i][j]);
        }
        cursorPos.Y++;
    }
}
void printTextCenter(char text[]) {
    HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(con, &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int x = (width - (int)strlen(text)) / 2;
    int y = height / 2;
    if (x < 0) x = 0;
    COORD pos = { x, y };
    SetConsoleCursorPosition(con, pos);
    puts(text);
}

void WhereText(int y, int x, const char text[]) {
    if (y < 1 || y > 28) return;
    for (int i = 0; text[i] != '\0' && x + i < 119; i++) {
        if (x + i > 0) Map[y][x + i] = text[i];
    }
}

void Upgrade() {
    system("cls");

Cont:
    mapEdge();

    char text[200];
    int cost1 = 1 + playData.cat1;
    int cost2 = 2 + playData.cat2;
    int cost3 = 2 + playData.cat3;
    int costMoney = 3 + (playData.moneySpeed - 6) * 2;
    if (costMoney < 3) costMoney = 3;

    snprintf(text, sizeof(text), "Text Power : %d", playData.Text_Power);
    WhereText(2, 4, text);
    WhereText(4, 4, "어떤 특수문자/능력을 업그레이드할지 고르세요");
    snprintf(text, sizeof(text), "1번 : @ 강화  현재:%d  비용:%d", playData.cat1, cost1);
    WhereText(6, 4, text);
    snprintf(text, sizeof(text), "2번 : & 강화  현재:%d  비용:%d", playData.cat2, cost2);
    WhereText(7, 4, text);
    snprintf(text, sizeof(text), "3번 : # 강화  현재:%d  비용:%d", playData.cat3, cost3);
    WhereText(8, 4, text);
    snprintf(text, sizeof(text), "4번 : 돈 수급 강화  현재:%d  비용:%d", playData.moneySpeed, costMoney);
    WhereText(9, 4, text);
    WhereText(11, 4, "0번 : 나가기");
    WhereText(13, 4, "입력 : ");
    PrintMap();

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(handle, &csbi);
    int conwidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int conheight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int startx = (conwidth - 120) / 2;
    int starty = (conheight - 30) / 2;
    if (startx < 0) startx = 0;
    if (starty < 0) starty = 0;
    COORD cursorPos;
    cursorPos.X = startx + 11;
    cursorPos.Y = starty + 13;
    SetConsoleCursorPosition(handle, cursorPos);

    int n;
    scanf("%d", &n);
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    if (n < 0 || n > 4) {
        WhereText(15, 4, "입력이 잘못되었습니다");
        PrintMap();
        Sleep(700);

        goto Cont;
    }

    if (n == 0) return;
    if (n == 1) {
        if (playData.Text_Power >= cost1) {
            playData.Text_Power -= cost1;
            playData.cat1 += 1;
            PlayData();
            WhereText(15, 4, "@ 강화 성공!");
        }
        else {
            WhereText(15, 4, "Text Power가 부족합니다");
        }
    }
    else if (n == 2) {
        if (playData.Text_Power >= cost2) {
            playData.Text_Power -= cost2;
            playData.cat2 += 2;
            PlayData();
            WhereText(15, 4, "& 강화 성공!");
        }
        else {
            WhereText(15, 4, "Text Power가 부족합니다");
        }
    }
    else if (n == 3) {
        if (playData.Text_Power >= cost3) {
            playData.Text_Power -= cost3;
            playData.cat3 += 2;
            PlayData();
            WhereText(15, 4, "# 강화 성공!");
        }
        else {
            WhereText(15, 4, "Text Power가 부족합니다");
        }
    }
    else if (n == 4) {
        if (playData.Text_Power >= costMoney) {
            playData.Text_Power -= costMoney;
            playData.moneySpeed += 1;
            PlayData();
            WhereText(15, 4, "돈 수급 강화 성공!");
        }
        else {
            WhereText(15, 4, "Text Power가 부족합니다");
        }
    }
    else {
        WhereText(15, 4, "입력이 잘못되었습니다");
    }
    PrintMap();
    Sleep(700);
    goto Cont;
}

void InGame() {
    SoundPlay(6);
    cat load[MAX_UNIT] = { 0 };
    cat enemy[MAX_UNIT] = { 0 };

    int money = 5;
    int PlayerCastleHp = 20 + (selectedStage - 1) * 5;
    int EnemyCastleHp = 1 + (selectedStage - 1) * 15;
    int tick = 0;
    int SpawnCool = 0, EnemySpawnCool = 0;
    int GameEnd = 0, win = 0;

    system("cls");

    while (1) {
        tick++;
        if (tick % 1 == 0 && money < 1000) money += playData.moneySpeed;
        if (SpawnCool > 0) SpawnCool--;
        if (EnemySpawnCool > 0) EnemySpawnCool--;
        int cost = 0;
        cat NewCat = { 0 };


        // 키입력처리
        if (SpawnCool == 0 && (GetAsyncKeyState('1') & 0x8000)) {
            NewCat = (cat){ 4 + playData.cat1, 1, 92, 22, '@', 1 };
            cost = 50;
        }
        else if (SpawnCool == 0 && (GetAsyncKeyState('2') & 0x8000)) {
            NewCat = (cat){ 7 + playData.cat2, 1, 92, 22, '&', 1 };
            cost = 100;
        }
        else if (SpawnCool == 0 && (GetAsyncKeyState('3') & 0x8000)) {
            NewCat = (cat){ 3 + playData.cat3, 2, 92, 22, '#', 1 };
            cost = 200;
        }

        // 플레이어가 버튼 누른걸 킵해두었을때만 발동을 하면서 돈이 그보다 많으면 발동한다.
        if (cost > 0 && money >= cost) {
            for (int i = 0; i < MAX_UNIT; i++) {
                if (load[i].alive == 0) {
                    load[i] = NewCat;
                    money -= cost;
                    SpawnCool = 4;
                    break;
                }
            }
        }

        if (EnemySpawnCool == 0) {
            cat NewEnemy;
            int r = rand() % 3;
            if (selectedStage == 1) {
                if (r == 0) NewEnemy = (cat){ 3, 1, 26, 22, 'm', 1 };
                else if (r == 1) NewEnemy = (cat){ 5, 1, 26, 22, 'w', 1 };
                else NewEnemy = (cat){ 2, 2, 26, 22, 'x', 1 };
                EnemySpawnCool = 18 + rand() % 15;
            }
            else if (selectedStage == 2) {
                if (r == 0) NewEnemy = (cat){ 5, 1, 26, 22, 'M', 1 };
                else if (r == 1) NewEnemy = (cat){ 7, 1, 26, 22, 'W', 1 };
                else NewEnemy = (cat){ 4, 2, 26, 22, 'X', 1 };
                EnemySpawnCool = 13 + rand() % 12;
            }
            else {
                if (r == 0) NewEnemy = (cat){ 7, 2, 26, 22, 'M', 1 };
                else if (r == 1) NewEnemy = (cat){ 10, 2, 26, 22, 'W', 1 };
                else NewEnemy = (cat){ 6, 3, 26, 22, 'X', 1 };
                EnemySpawnCool = 9 + rand() % 10;
            }

            for (int i = 0; i < MAX_UNIT; i++) {
                if (enemy[i].alive == 0) {
                    enemy[i] = NewEnemy;
                    break;
                }
            }
        }
        for (int i = 0; i < MAX_UNIT; i++) {
            if (load[i].alive == 0) continue;
            for (int j = 0; j < MAX_UNIT; j++) {
                if (enemy[j].alive == 0) continue;
                if (load[i].x <= enemy[j].x + 1 && load[i].x >= enemy[j].x - 1) {
                    enemy[j].hp -= load[i].atk;
                    load[i].hp -= enemy[j].atk;
                    if (enemy[j].hp <= 0) {
                        enemy[j].alive = 0;
                        if (money < 99) money++;
                    }
                    if (load[i].hp <= 0) load[i].alive = 0;
                    break;
                }
            }
        }
        for (int i = 0; i < MAX_UNIT; i++) {
            if (load[i].alive) {
                int blocked = 0;
                for (int j = 0; j < MAX_UNIT; j++) {
                    if (enemy[j].alive && load[i].x <= enemy[j].x + 1 && load[i].x >= enemy[j].x - 1) {
                        blocked = 1;
                        break;
                    }
                }
                if (!blocked) load[i].x--;
                if (load[i].x <= 25) {
                    load[i].alive = 0;
                    EnemyCastleHp -= load[i].atk;
                }
            }
            if (enemy[i].alive) {
                int blocked = 0;
                for (int j = 0; j < MAX_UNIT; j++) {
                    if (load[j].alive && enemy[i].x <= load[j].x + 1 && enemy[i].x >= load[j].x - 1) {
                        blocked = 1;
                        break;
                    }
                }
                if (!blocked) enemy[i].x++;
                if (enemy[i].x >= 93) {
                    enemy[i].alive = 0;
                    PlayerCastleHp -= enemy[i].atk;
                }
            }
        }
        if (EnemyCastleHp <= 0) {
            GameEnd = 1;
            win = 1;
        }
        if (PlayerCastleHp <= 0) {
            GameEnd = 1;
            win = 0;
        }


        // UI 처리 인데 그냥 정리한다 정도
        mapEdge();
        LoadCastle();

        char hud[200];
        snprintf(hud, sizeof(hud), "STAGE:%d   플레이어: %s   돈:%02d   내 성_체력:%02d   적 성_체력:%02d", selectedStage, player, money, PlayerCastleHp, EnemyCastleHp);
        WhereText(2, 4, hud);
        WhereText(4, 4, "1: 일반 문(@, cost 50)   2: 탱크 문(&, cost 100)   3: 전투 문(#, cost 200)   ESC: 탈출");
        WhereText(21, 27, "적 성");
        WhereText(21, 90, "내 성");

        for (int i = 0; i < MAX_UNIT; i++) {
            if (load[i].alive && load[i].x > 25 && load[i].x < 93) Map[load[i].y][load[i].x] = load[i].shape;
            if (enemy[i].alive && enemy[i].x > 25 && enemy[i].x < 93) Map[enemy[i].y][enemy[i].x] = enemy[i].shape;
        }

        PrintMap();

        if (GameEnd) {
            Sleep(700);
            system("cls");
            char text[1000] = "승리! 적 성을 무너뜨렸습니다.";
            if (win) {
                int reward = selectedStage * 3;
                playData.Text_Power += reward;
                if (playData.stage == selectedStage && playData.stage < 3) playData.stage++;
                PlayData();
                snprintf(text, sizeof(text), "승리! STAGE %d 클리어! Text Power +%d", selectedStage, reward);
                printTextCenter(text);
                SoundPlay(2);
            }
            else {
                strcpy(text, "패배... 우리 성이 무너졌습니다.");
                printTextCenter(text);
                SoundPlay(3);
            }
            Sleep(20000);
            break;
        }

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            system("cls");
            char text[100] = "게임을 종료합니다.";
            printTextCenter(text);
            Sleep(1000);
            break;
        }

        Sleep(100);
    }
}

void Loading()
{
    Gethandle();
    for (int j = 0; j < 3; j++) {
        char text[100] = "Loading";
        printTextCenter(text);
        Sleep(250);
        for (int i = 0; i < 3; i++) {
            strcat(text, ".");
            printTextCenter(text);
            Sleep(250);
        }
        system("cls");
    }
    srand(time(NULL));
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(handle, &csbi);
    int conwidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int conheight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int startx = (conwidth - 120) / 2;
    int starty = (conheight - 30) / 2;
    if (startx < 0) startx = 0;
    if (starty < 0) starty = 0;
    COORD cursorPos;
    cursorPos.X = startx;
    cursorPos.Y = starty;
    for (int i = 0; i < 120; i++) {
        COORD cursorPos;
        cursorPos.X = i + startx;
        for (int j = 0; j < 30; j++) {
            cursorPos.Y = j + starty;
            SetConsoleCursorPosition(handle, cursorPos);
            if (j % 3 == 0) {
                printf("*");
            }
            if (i % 3 == 1) {
                printf("'");
            }
            else printf(" ");
        }
        Sleep(5);
    }
    system("cls");
}
void SoundPlay(int a) {
    if (a == 0) PlaySound(NULL, 0, 0);
    if (a == 1) PlaySound(TEXT("MainBGM.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    if (a == 2) PlaySound(TEXT("Result.wav"), NULL, SND_FILENAME | SND_ASYNC);
    if (a == 3) PlaySound(TEXT("Fail.wav"), NULL, SND_FILENAME | SND_ASYNC);
    if (a == 4) PlaySound(TEXT("click.wav"), NULL, SND_FILENAME | SND_ASYNC);
    if (a == 5) PlaySound(TEXT("fight.wav"), NULL, SND_FILENAME | SND_ASYNC);
    if (a == 6) PlaySound(TEXT("Ingame.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
