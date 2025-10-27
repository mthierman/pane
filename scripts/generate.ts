import { Temporal } from "@js-temporal/polyfill";
import { writeFile } from "node:fs/promises";
import { dirname, join } from "node:path";
import package_json from "../package.json" with { type: "json" };

const config = {
    event_type: "pane-notify",
    client_payload: {
        name: package_json.name,
        version: package_json.version,
        description: package_json.description,
        date: Temporal.Now.plainDateTimeISO(),
        hash: "a4d0cb71-cdee-48c0-bfd4-4dedbb007027",
        symbol: "🪟",
        github: "https://github.com/mthierman/pane",
        download: "https://github.com/mthierman/pane/releases",
    },
};

const data = join(dirname(import.meta.dirname), "data");
await writeFile(join(data, "dispatch.json"), JSON.stringify(config, null, 4), "utf8");
