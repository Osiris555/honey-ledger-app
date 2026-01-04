const TransportNodeHid = require("@ledgerhq/hw-transport-node-hid").default;

/*
 APDU FORMAT (WHAT WE SEND):

 CLA | INS | P1 | P2 | LC | DATA...

 CLA = 0xE0 (custom app)
 INS = 0x02 (SIGN_TX)
 P1  = 0x00
 P2  = 0x00
*/

async function run() {
    const transport = await TransportNodeHid.create();

    // ---- Example transaction payload ----
    // You MUST match what your Ledger parser expects
    const tx = Buffer.from([
        // to address (20 bytes)
        0x11,0x22,0x33,0x44,0x55,
        0x66,0x77,0x88,0x99,0xaa,
        0xbb,0xcc,0xdd,0xee,0xff,
        0x00,0x11,0x22,0x33,0x44,

        // amount (uint64, big endian)
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x03,0xE8, // = 1000 wei (example)

        // chain id (uint32)
        0x00,0x00,0x00,0x01,

        // nonce (8 bytes)
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x01
    ]);

    try {
        const response = await transport.send(
            0xE0,   // CLA
            0x02,   // INS (SIGN_TX)
            0x00,   // P1
            0x00,   // P2
            tx
        );

        console.log("Ledger response:", response.toString("hex"));
    } catch (e) {
        console.error("Ledger error:", e);
    } finally {
        transport.close();
    }
}

run();
