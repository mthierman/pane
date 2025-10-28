import { Temporal } from "@js-temporal/polyfill";
import { Octokit } from "@octokit/rest";
import { execSync } from "node:child_process";
import package_json from "../package.json" with { type: "json" };

// https://docs.github.com/en/rest/guides/scripting-with-the-rest-api-and-javascript?apiVersion=2022-11-28
const octokit = new Octokit({ auth: process.env.GH_TOKEN });
await octokit.repos.createDispatchEvent({
    owner: package_json.pane.owner,
    repo: package_json.pane.web_repo,
    event_type: package_json.pane.event_types.notify,
    client_payload: {
        name: package_json.name,
        version: package_json.version,
        description: package_json.description,
        date: Temporal.Now.plainDateTimeISO(),
        hash: execSync(`git rev-parse HEAD`, { encoding: "utf-8" }),
        symbol: "🪟",
        github: package_json.homepage,
        download: package_json.pane.releases,
        commits: execSync(`git log -5 --pretty=format:%h%x00%an%x00%aI%x00%s%x00`, {
            encoding: "utf-8",
        })
            .split("\n")
            .filter(Boolean)
            .map((line) => {
                const [commit, author, date, message] = line.split("\x00");
                return { commit, author, date, message };
            }),
    },
});
