import TransportNodeHid from "@ledgerhq/hw-transport-node-hid";

const CLA = 0xE0;
const INS_SIGN_TX = 0x04;

const P1_FIRST = 0x00;
const P1_MORE  = 0x01;
const P1_LAST  = 0x02;

/*
 Transaction format (28 bytes total):
 [0..19]  recipient address (20 bytes)
 [20..27] amount (uint64 BE, 18 decimals)
*/

function buildTransaction() {
  const recipient = Buffer.alloc(20, 0x11); // dummy address
  const amount = BigInt("1234500000000000000"); // 1.2345 HONEY

  const amountBuf = Buffer.alloc(8);
  amountBuf.writeBigUInt64BE(amount);

  return Buffer.concat([recipient, amountBuf]);
}

async function main() {
  const transport = await TransportNodeHid.open("");

  const tx = buildTransaction();

  // FIRST + LAST since tx is small
  console.log("Sending transaction...");
  const response = await transport.send(
    CLA,
    INS_SIGN_TX,
    P1_FIRST,
    0x00,
    tx
  );

  console.log("Ledger response:", response.toString("hex"));

  await transport.close();
}

main().catch(err => {
  console.error(err);
  process.exit(1);
});
