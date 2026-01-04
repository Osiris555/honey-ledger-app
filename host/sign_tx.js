import TransportNodeHid from "@ledgerhq/hw-transport-node-hid";

const CLA = 0xE0;
const INS_SIGN_TX = 0x04;

const P1_FIRST = 0x00;

/*
 Transaction format (28 bytes):
 [0..19] recipient (20 bytes)
 [20..27] amount uint64 BE (18 decimals)
*/

function buildTransaction() {
  const recipient = Buffer.alloc(20, 0x11);
  const amount = BigInt("1234500000000000000"); // 1.2345 HONEY

  const amountBuf = Buffer.alloc(8);
  amountBuf.writeBigUInt64BE(amount);

  return Buffer.concat([recipient, amountBuf]);
}

/*
 Parse DER ECDSA signature:
 30 len
   02 len_r r
   02 len_s s
*/
function parseDERSignature(der) {
  if (der[0] !== 0x30) {
    throw new Error("Invalid DER header");
  }

  let offset = 2;

  if (der[offset] !== 0x02) {
    throw new Error("Invalid R marker");
  }

  const rLen = der[offset + 1];
  const r = der.slice(offset + 2, offset + 2 + rLen);
  offset += 2 + rLen;

  if (der[offset] !== 0x02) {
    throw new Error("Invalid S marker");
  }

  const sLen = der[offset + 1];
  const s = der.slice(offset + 2, offset + 2 + sLen);

  return {
    r: r.toString("hex"),
    s: s.toString("hex")
  };
}

async function main() {
  const transport = await TransportNodeHid.open("");

  const tx = buildTransaction();

  console.log("Sending transaction to Ledger…");

  const response = await transport.send(
    CLA,
    INS_SIGN_TX,
    P1_FIRST,
    0x00,
    tx
  );

  // Last 2 bytes are SW (9000)
  const derSig = response.slice(0, response.length - 2);

  console.log("DER signature:", derSig.toString("hex"));

  const { r, s } = parseDERSignature(derSig);

  console.log("Parsed signature:");
  console.log("r =", r);
  console.log("s =", s);

  await transport.close();
}

main().catch(err => {
  console.error(err);
  process.exit(1);
});
