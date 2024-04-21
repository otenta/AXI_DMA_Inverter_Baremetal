#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "xaxidma.h"
#include "xparameters.h"
#include "xil_cache.h"
#include "xil_printf.h"

u32 checkHalted(u32 baseAddress, u32 offset);

// Define semaphore handle
SemaphoreHandle_t dmaSemaphore;

void dmaTask(void *pvParameters) {
	//400 Bytes of data
	u32 a[] = {
	    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
	    31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	    41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
	    51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
	    61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
	    71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
	    81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
	    91, 92, 93, 94, 95, 96, 97, 98, 99, 100
	};
    u32 b[100];
    u32 status;

    XAxiDma_Config *myDmaConfig;
    XAxiDma myDma;

    // Initialize DMA
    myDmaConfig = XAxiDma_LookupConfigBaseAddr(XPAR_AXI_DMA_0_BASEADDR);
    status = XAxiDma_CfgInitialize(&myDma, myDmaConfig);
    if (status != XST_SUCCESS) {
        xil_printf("DMA initialization failed\n");
    }
    xil_printf("DMA initialization success..\n");

    // Flush source and destination buffers before DMA transfer
    Xil_DCacheFlushRange((u32)a, sizeof(a));
    Xil_DCacheFlushRange((u32)b, sizeof(b));

    // Perform DMA transfer
    status = XAxiDma_SimpleTransfer(&myDma, (u32)b, sizeof(b), XAXIDMA_DEVICE_TO_DMA);
    status |= XAxiDma_SimpleTransfer(&myDma, (u32)a, sizeof(a), XAXIDMA_DMA_TO_DEVICE);
    if (status != XST_SUCCESS) {
        xil_printf("DMA transfer failed\n");
    }

    // Wait for DMA transfer to complete
    while (checkHalted(XPAR_AXI_DMA_0_BASEADDR, 0x4) != 1);
    while (checkHalted(XPAR_AXI_DMA_0_BASEADDR, 0x34) != 1);

    // Invalidate destination buffer after DMA transfer
    Xil_DCacheInvalidateRange((u32)b, sizeof(b));

    // Print transferred data
    for (int i = 0; i < 100; i++) {
        xil_printf("%0x\n", b[i]);
    }

    // Clean up
    vTaskDelete(NULL);
}

int main() {
    xil_printf("FreeRTOS AXI DMA Example\n");

    // Create DMA task
    xTaskCreate(dmaTask, "DMATask", configMINIMAL_STACK_SIZE*2, NULL, tskIDLE_PRIORITY + 1, NULL);

    // Start scheduler
    vTaskStartScheduler();

    // Should not reach here
    while (1);

    return 0;
}

u32 checkHalted(u32 baseAddress, u32 offset) {
    u32 status;
    status = (XAxiDma_ReadReg(baseAddress, offset)) & XAXIDMA_HALTED_MASK;
    return status;
}
