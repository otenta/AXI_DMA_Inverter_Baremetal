First we need to create the inverter in verilog and add it to a ipRepo dir. 
After writing the verilog part, click Tools-> Create and Package IP 
Click next and "Package your current project". Choose default and click next.
Now in the Package IP screen, go to "Ports and Interfaces".
Click on "Add". For the interface definition choose axis_rtl and click OK.
For the name add "m_axis" and choose master in Mode. Then go to Port Mapping.
Select TVALID and map it to m_axis_valid
Select TREADY and map it to m_axis_ready
Select TDATA and map it to m_axis_data
Then click OK
Now do the same process for "s_axis" and configure it in slave mode.
After that, in the Package IP Screen, double click on axi_clk
Select "Parameters" and click on Add.
Add "ASSOCIATED_BUSIF" and in the value column add "m_axis:s_axis"
Click ok
Add another parameter like "FREQ_HZ" with the specified frequency (100MHZ = 100000000)
Click ok
Now go to Review and Package and select "Package IP"
Now this custom IP is ready to be used in our other project with the AXI DMA.

///Write process for AXI DMA connections and ILA debug as well

