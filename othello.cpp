#include "DxLib.h"
#include <string>

// 関数のプロトタイプ宣言
void syoki_rtn();
void sente_rtn();
void gote_rtn();
void hantei_rtn();

// グローバル変数
int i, j, x = 1, y = 1, player = 0; // 初期値を設定
int maisu0, maisu1;
int goban[10][10];

int WINAPI WinMain(HINSTANCE, HINSTANCE hP, LPSTR lpC, int nC)
{
    ChangeWindowMode(TRUE); // ウィンドウモード
    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK); // バックバッファに描画
    syoki_rtn();

    /* 対戦処理 */
    while (1)
    {
        if (ProcessMessage() == -1) break;
        if (ClearDrawScreen() == -1) break;
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;

        // カーソル移動
        if (CheckHitKey(KEY_INPUT_LEFT) == 1)
        {
            if (x > 1) x--;
            WaitTimer(250);
        }
        if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
        {
            if (x < 8) x++;
            WaitTimer(250);
        }
        if (CheckHitKey(KEY_INPUT_UP) == 1)
        {
            if (y > 1) y--;
            WaitTimer(250);
        }
        if (CheckHitKey(KEY_INPUT_DOWN) == 1)
        {
            if (y < 8) y++;
            WaitTimer(250);
        }
        if (CheckHitKey(KEY_INPUT_RETURN) == 1)
        {
            if (player == 0)
                sente_rtn();
            else
                gote_rtn();
            WaitTimer(250);// 石を置いた後の待機
        }

        hantei_rtn();
        ScreenFlip(); // 画面更新
    }

    WaitKey(); // ループ終了後にキー入力を待つ
    DxLib_End();
    return 0;
}

/* 初期化 */
void syoki_rtn()
{
    // 盤面の初期化
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            goban[i][j] = 3; // 枠外
    for (i = 1; i < 9; i++)
        for (j = 1; j < 9; j++)
            goban[i][j] = 2; // 空きマス


    // 初期配置
    goban[5][4] = 0; // 黒
    goban[4][5] = 0; // 黒
    goban[4][4] = 1; // 白
    goban[5][5] = 1; // 白
}

/* 先手処理 */
void sente_rtn()
{
    if (goban[x][y] == 2) // 空きマスの場合のみ
    {
        goban[x][y] = 0; // 黒を置く

        // 8方向を定義
        int directions[8][2] = { {-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1} };

        // 各方向で挟む白石をチェックしてひっくり返す
        for (int d = 0; d < 8; d++)
        {
            int dx = directions[d][0];
            int dy = directions[d][1];
            int cx = x + dx;
            int cy = y + dy;
            int count = 0; // 挟まれる白石の数

            // 隣接する白石をカウント
            while (cx >= 1 && cx <= 8 && cy >= 1 && cy <= 8 && goban[cx][cy] == 1)
            {
                count++;
                cx += dx;
                cy += dy;
            }

            // その先に黒石（0）があれば、間の白石をひっくり返す
            if (cx >= 1 && cx <= 8 && cy >= 1 && cy <= 8 && goban[cx][cy] == 0 && count > 0)
            {
                cx = x + dx;
                cy = y + dy;
                for (int i = 0; i < count; i++)
                {
                    goban[cx][cy] = 0; // 白を黒に変更
                    cx += dx;
                    cy += dy;
                }
            }
        }

        player = 1; // 後手に交代
    }
}

/* 後手処理 */
void gote_rtn()
{
    if (goban[x][y] == 2) // 空きマスの場合のみ
    {
        goban[x][y] = 1; // 白を置く

        // 8方向を定義
        int directions[8][2] = { {-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1} };

        // 各方向で挟む黒石をチェックしてひっくり返す
        for (int d = 0; d < 8; d++)
        {
            int dx = directions[d][0];
            int dy = directions[d][1];
            int cx = x + dx;
            int cy = y + dy;
            int count = 0; // 挟まれる黒石の数

            // 隣接する黒石をカウント
            while (cx >= 1 && cx <= 8 && cy >= 1 && cy <= 8 && goban[cx][cy] == 0)
            {
                count++;
                cx += dx;
                cy += dy;
            }

            // その先に白石（1）があれば、間の黒石をひっくり返す
            if (cx >= 1 && cx <= 8 && cy >= 1 && cy <= 8 && goban[cx][cy] == 1 && count > 0)
            {
                cx = x + dx;
                cy = y + dy;
                for (int i = 0; i < count; i++)
                {
                    goban[cx][cy] = 1; // 黒を白に変更
                    cx += dx;
                    cy += dy;
                }
            }
        }

        player = 0; // 先手に交代
    }
}

/* 判定処理と描画 */
void hantei_rtn()
{
    // 背景と盤面の描画
    DrawBox(0, 0, 640, 640, 0xFFAAAA, TRUE); // 背景
    DrawString(230, 10, "othello", 0x0000FF);
    // 背景と盤面の描画
    DrawBox(0, 0, 640, 640, 0xFFAAAA, TRUE); // 背景
    DrawString(230, 10, "othello", 0x0000FF);

    DrawString(500, 50, "sente", 0x0000FF);
    DrawCircle(570, 55, 20, 0x000000);
    DrawString(500, 100, "gote", 0x0000FF);
    DrawCircle(570, 105, 20, 0xFFFFFF);

    DrawString(500, 150, "up key", 0x0000FF);
    DrawString(500, 200, "down key", 0x0000FF);
    DrawString(500, 250, "left key", 0x0000FF);
    DrawString(500, 300, "right key", 0x0000FF);
    DrawString(500, 350, "done Enterkey", 0x0000FF);


    // 盤面の描画
    for (i = 1; i < 9; i++)
    {
        for (j = 1; j < 9; j++)
        {
            DrawBox(i * 50, j * 50, i * 50 + 50, j * 50 + 50, 0xFFAAAA, TRUE); // マス
            if (goban[i][j] == 0)
            {
                DrawCircle(i * 50 + 25, j * 50 + 25, 20, 0x000000, TRUE); // 黒
                DrawCircle(i * 50 + 25, j * 50 + 25, 20, 0xFFFFFF, FALSE);
            }
            else if (goban[i][j] == 1)
            {
                DrawCircle(i * 50 + 25, j * 50 + 25, 20, 0xFFFFFF, TRUE); // 白
                DrawCircle(i * 50 + 25, j * 50 + 25, 20, 0x000000, FALSE);
            }
        }
    }

    // 緑色の線を引く（縦線と横線）
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            DrawBox(i * 50 + 50, j * 50 + 50, (i + 1) * 50 + 50, (j + 1) * 50 + 50, 0x00FF00, FALSE);
        }
    }

    // カーソルの描画
    DrawBox(x * 50, y * 50, x * 50 + 50, y * 50 + 50, 0x000000, FALSE); // 黄色の枠

    // 石のカウント
    maisu0 = maisu1 = 0;
    for (i = 1; i < 9; i++)
    {
        for (j = 1; j < 9; j++)
        {
            if (goban[i][j] == 0) maisu0++;
            if (goban[i][j] == 1) maisu1++;
        }
    }

    // 終了判定
    if (maisu0 + maisu1 == 64)
    {
        if (maisu0 > maisu1)
        {
            DrawString(0, 20, "sente win", 0x0000FF);
        }
        else if (maisu0 < maisu1)
        {
            DrawString(0, 20, "gote win", 0x0000FF);
        }
        else
        {
            DrawString(0, 20, "draw", 0x0000FF);
        }
    }
}