import { mkdir } from "fs";
import { writeFile } from "node:fs/promises";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const data = join(dirname(import.meta.dirname), "data");

console.log(data);

const config = {
    event_type: "pane-notify",
    client_payload: {
        name: "pane",
        version: "0.0.0",
        description: "C++ library for Windows",
        date: "10/07/24 20:59:51",
        hash: "a4d0cb71-cdee-48c0-bfd4-4dedbb007027",
        symbol: "🪟",
        github: "https://github.com/mthierman/pane",
        download: "https://github.com/mthierman/pane/releases",
    },
};

await writeFile(join(data, "dispatch.json"), JSON.stringify(config, null, 2), "utf8");
