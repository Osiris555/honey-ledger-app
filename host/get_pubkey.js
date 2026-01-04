import TransportNodeHid from "@ledgerhq/hw-transport-node-hid";

const CLA = 0xE0;
const INS_GET_PUBLIC_KEY = 0x02;

async function main() {
  const transport = await TransportNodeHid.open("");

  console.log("Requesting public key…");

  const response = await transport.send(
    CLA,
    INS_GET_PUBLIC_KEY,
    0x00,
    0x00
  );

  const data = response.slice(0, response.length - 2);

  const pubkey = data.slice(0, 33);
  const address = data.slice(33, 53);

  console.log("Public key:", pubkey.toString("hex"));
  console.log("Honey address:", address.toString("hex"));

  await transport.close();
}

main().catch(err => {
  console.error(err);
  process.exit(1);
});
