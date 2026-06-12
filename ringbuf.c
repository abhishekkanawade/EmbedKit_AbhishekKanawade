
#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 8
typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    int head;
    int tail;
    int count;
} RingBuffer;

/* Initialize Ring Buffer */
void initBuffer(RingBuffer *rb)
{
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

/* Check Full */
int isFull(RingBuffer *rb)
{
    return (rb->count == BUFFER_SIZE);
}

/* Check Empty */
int isEmpty(RingBuffer *rb)
{
    return (rb->count == 0);
}

/* Write Data */
int writeBuffer(RingBuffer *rb, uint8_t data)
{
    if (isFull(rb))
    {
        return -1;
    }

    rb->buffer[rb->head] = data;

    rb->head++;

    if (rb->head == BUFFER_SIZE)
    {
        rb->head = 0;
    }

    rb->count++;

    return 0;
}
/* Read Data */
int readBuffer(RingBuffer *rb, uint8_t *data)
{
    if (isEmpty(rb))
    {
        return -1;
    }

    *data = rb->buffer[rb->tail];

    rb->tail++;

    if (rb->tail == BUFFER_SIZE)
    {
        rb->tail = 0;
    }

    rb->count--;

    return 0;
}

int main()
{
    RingBuffer rb;
    uint8_t data;
    int i;

    initBuffer(&rb);

    /* Step 1: Write 8 bytes */
 for (i = 0; i < 8; i++)
    {
        data = 0x41 + i;

        if (writeBuffer(&rb, data) == 0)
        {
            printf("[WRITE] 0x%02X -> OK (count=%d)",
                   data,
                   rb.count);

            if (isFull(&rb))
            {
                printf(" FULL");
            }

            printf("\n");
        }
    }

    /* Step 2: Write one extra byte */
    if (writeBuffer(&rb, 0x99) == -1)
    {
        printf("[WRITE] 0x99 -> FAIL (buffer full)\n");
    }
 /* Step 3: Read 3 bytes */
    for (i = 0; i < 3; i++)
    {
        if (readBuffer(&rb, &data) == 0)
        {
            printf("[READ] -> 0x%02X (count=%d)\n",
                   data,
                   rb.count);
        }
    }

    /* Step 4: Write 3 more bytes */
    uint8_t newData[] = {0x49, 0x4A, 0x4B};

    for (i = 0; i < 3; i++)
    {
        if (writeBuffer(&rb, newData[i]) == 0)
        {
            printf("[WRITE] 0x%02X -> OK (count=%d)\n",
                   newData[i],
                   rb.count);
        }
    }
 /* Step 5: Read remaining bytes */
    while (!isEmpty(&rb))
    {
        if (readBuffer(&rb, &data) == 0)
        {
            printf("[READ] -> 0x%02X (count=%d)\n",
                   data,
                   rb.count);
        }
    }

    printf("Buffer Empty\n");

    /* Step 6: Read from empty buffer */
    if (readBuffer(&rb, &data) == -1)
    {
        printf("[READ] (empty) -> FAIL (buffer empty)\n");
    }

    return 0;
}
