#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <gmp.h>

#define NUM_THREADS 20       // スレッド数
#define PRINT_INTERVAL 10000 // 途中経過を表示する間隔

typedef struct
{
    int start;
    int end;
    mpz_t result;
} ThreadData;

void *factorial_partial(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    mpz_set_ui(data->result, 1); // 初期値を1に設定

    for (int i = data->start; i <= data->end; i++)
    {
        mpz_mul_ui(data->result, data->result, i);

        // 一定の桁数ごとに途中経過の桁数を出力
        if (i % PRINT_INTERVAL == 0)
        {
            size_t digits = mpz_sizeinbase(data->result, 10); // gmpから桁数取得
            printf("途中経過 (i=%d): 桁数 = %zu\n", i, digits);
        }
    }

    pthread_exit(NULL);
}

int main()
{
    int n;
    printf("nを入力してください: ");
    scanf("%d", &n);

    if (n < 0)
    {
        printf("負の数もしくはintのオーバーフローの階乗は計算できません\n");
        return 1;
    }

    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];

    int range = n / NUM_THREADS;
    int start = 1;

    // スレッドごとに範囲を設定
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threadData[i].start = start;
        threadData[i].end = (i == NUM_THREADS - 1) ? n : (start + range - 1);
        mpz_init(threadData[i].result);
        pthread_create(&threads[i], NULL, factorial_partial, (void *)&threadData[i]);
        start += range;
    }

    // スレッドの終了を待機
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // 各スレッドの結果をマージ
    mpz_t totalResult;
    mpz_init_set_ui(totalResult, 1); // 初期値1

    for (int i = 0; i < NUM_THREADS; i++)
    {
        mpz_mul(totalResult, totalResult, threadData[i].result);
        mpz_clear(threadData[i].result); // メモリ解放
    }

    // 結果をファイルに書き出し
    FILE *fp = fopen("factorial_result.txt", "w");
    if (fp != NULL)
    {
        fprintf(fp, "n = %d の階乗は:\n", n);
        mpz_out_str(fp, 10, totalResult); // gmpからstringに変換
        fprintf(fp, "\n");
        fclose(fp);
    }
    else
    {
        perror("ファイルオープンエラー");
    }

    mpz_clear(totalResult);
    pthread_exit(NULL);

    return 0;
}
