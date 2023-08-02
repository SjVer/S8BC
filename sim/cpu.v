module CPU ();

// wires
reg clk, rst;

// control signals
reg 
    pci,
    pcl_db, pcl_abl, abl_pcl,
    pch_db, pch_abh, abh_pch;

// busses
wire [7:0] db;
wire [7:0] idb;
wire [7:0] abl;
wire [7:0] abh;

// components
PC PC(
    clk, rst,
    pci,
    pcl_db, pcl_abl, abl_pcl,
    pch_db, pch_abh, abh_pch,
    db, abl, abh
);

// logic

initial begin 
    clk <= 0;

    // initialize signals 
    {
        pci,
        pcl_db, pcl_abl, abl_pcl,
        pch_db, pch_abh, abh_pch
    } <= 0;

    // trigger reset
    rst <= 1;
    rst <= 0;

    pci <= 1;
    pch_db <= 1;
end

always #1 clk = ~clk;

// always @* begin
    // $monitor ("DB: $%h", db);
    // $monitor ("IDB: $%h", idb);
    // $monitor ("ABL: $%h", abl);
    // $monitor ("ABH: $%h", abh);
// end

endmodule
