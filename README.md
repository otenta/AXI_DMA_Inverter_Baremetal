# PL image inverter

- First we need to create the inverter in verilog and add it to a ipRepo dir. 
- After writing the verilog part, click Tools-> Create and Package IP Click next and "Package your current project". 
- Choose default and click next. 
- Now in the Package IP screen, go to "Ports and Interfaces". 
- Click on "Add". For the interface definition choose axis_rtl and click OK. 
- For the name add "m_axis" and choose master in Mode. Then go to Port Mapping. 
- Select TVALID and map it to m_axis_valid 
- Select TREADY and map it to m_axis_ready 
- Select TDATA and map it to m_axis_data 
- Then click OK 
- Now do the same process for "s_axis" and configure it in slave mode. 
- After that, in the Package IP Screen, double click on axi_clk Select "Parameters" and click on Add. 
- Add "ASSOCIATED_BUSIF" and in the value column add "m_axis:s_axis" 
- Click ok 
- Add another parameter like "FREQ_HZ" with the specified frequency (100MHZ = 100000000) 
- Click ok 
- Now go to Review and Package and select "Package IP" 
- Now this custom IP is ready to be used in our other project with the AXI DMA.

# AXI DMA Connections to PS PL

- Create a new project. Select RTL project and click do not specify sources.
- Select your target board (ARTY Z7-20)
- And click finish
- Go to settings, IP and add your repository
- Now click on "Create block design"
- Add your zynq processing system.
- And run connection automation.
- Now add the custom IP from your repo by searching for "inverter"
- Now add the AXI DMA.
- Run connection automation to connect the S_AXI_LITE from the AXI DMA to the M_AXI_GP0 in order to configure the DMA.
- Connect the M_AXIS_MM2S of the DMA to the slave s_axis of the inverter.
- Now connect the master m_axis of the inverter to the S_AXIS_SS2M of the DMA.
- Double click on the AXI DMA and diasble the scatter gather mode.
- Run connection automation to connect the axi_clk of the inverter to the FCLK_CLK0 of the ZYNQ.
- Now connect the axi_reset of the inverter to the axi_resetn of the DMA.
- After that we need to connect the DMA to the ZYNQ for reading and writing data.
- To do that, double click on the ZYNQ and go to PS-PL Configuration.
- Click on HP Slave AXI Interface and enable one Slave High Performance AXI port.
- After that, run connection automation to connect the HP port to the M_AXI_MM2S of the DMA.
- Run connection automation again to connect the HP port to the M_AXI_SS2M of the DMA.
- You can check the memory mapped addresses in the "Address Editor" tab.
- Now select the interface signals you want to check and right click and select "Debug"
- The signals we want to check are m_axis and s_axis of the inverter,
- The M_AXI_MM2S and M_AXI_SS2M of the DMA and the S_AXI_HP0 port of the ZYNQ.
- Run connection automation and select the mem_interconnect.
- Run connection automation again and select all the interfaces.
- This will add an ILA IP block in order to check the signals of the interfaces.
- Click on the Design Sources tab and create HDL wrapper for your block design.
- After that go to Settings and enter the name of the HDL wrapper in top_module field.
- After that select Generate Bitstream.
- After this process completes, select File-> Export Hardware.

# AXI DMA baremetal application

- Open VITIS IDE and create a new platform project.
- Select the HDL wrapper and create the project on baremetal.
- After tha create an application project using the platform project you just created.
- Select empty application as a template.
- Then write the code as is in axi dma baremetal.c file.
- After that, go to Run-> Run configurations and disable the Program FPGA option (if you have selected the "Include Bitstream option during Export).
- Also disable the Reset Entire System option. 
- In Main tab select Debug Type -> Standalone Application Debug 


# AXI DMA Debug

- Now go to Vivado and click on Hardware Manager
- Click on Open target and run the auto option.
- Click on Program Device option in Hardware Manager.
- Now click on Trigger Setup and delete the default option.
- Add new trigger and select the M_AXIS_MM2S: TVALID signal
- Select R in value.
- Click on "Run trigger for this ILA core"
- Now go to Vitis SDK and run the application.
- Now go to Hardware Manager to check the signals.
