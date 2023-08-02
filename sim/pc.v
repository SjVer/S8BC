module PC (input 
    clk, rst,
    pci,
    pcl_db, pcl_abl, abl_pcl,
    pch_db, pch_abh, abh_pch,
    inout [7:0] db,
    inout [7:0] abl,
    inout [7:0] abh
);

// state
reg [15:0] pc;
wire [7:0] pcl = pc[7:0];
wire [7:0] pch = pc[15:8];

// logic

always @ (posedge rst)
    pc <= 0;

always @ (posedge clk) begin
    if (pc == 'hffff) $finish;

    if (pcl_db) pc <= {db, pch};
    if (pch_db) pc <= {pcl, db};
end

always @ (negedge clk) 
    if (pci) pc <= pc + 1;

always @*
    $monitor ("PC: $%h", pc);

// connections
assign db = pcl_db ? pcl : 'bz;
assign db = pch_db ? pch : 'bz;
assign abl = pcl_abl ? pcl : 'bz;
assign abh = pch_abh ? pch : 'bz;

endmodule
