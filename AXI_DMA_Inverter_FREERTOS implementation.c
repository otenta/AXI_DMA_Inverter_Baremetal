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
    u32 a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    u32 b[8];
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
    for (int i = 0; i < 8; i++) {
        xil_printf("%0x\n", b[i]);
    }

    // Clean up
    vTaskDelete(NULL);
}

int main() {
    xil_printf("FreeRTOS AXI DMA Example\n");

    // Create DMA task
    xTaskCreate(dmaTask, "DMATask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

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
