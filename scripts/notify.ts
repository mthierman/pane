import { Temporal } from "@js-temporal/polyfill";
import { mkdir, writeFile } from "node:fs/promises";
import { dirname, join } from "node:path";
import package_json from "../package.json" with { type: "json" };

// https://docs.github.com/en/rest/guides/scripting-with-the-rest-api-and-javascript?apiVersion=2022-11-28
const build_dir = join(dirname(import.meta.dirname), "build");
mkdir(build_dir, { recursive: true });
const event_type = "pane-notify";
const event = {
    event_type: event_type,
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
await writeFile(join(build_dir, `${event_type}.json`), JSON.stringify(event, null, 4), "utf8");
