import { Temporal } from "@js-temporal/polyfill";
import { Octokit } from "@octokit/rest";
import { spawnSync } from "node:child_process";
import package_json from "../package.json" with { type: "json" };

const payload = {
    event_type: package_json.pane.event_types.notify,
    client_payload: {
        [package_json.name]: {
            name: package_json.name,
            version: package_json.version,
            description: package_json.description,
            build_time: Temporal.Now.plainDateTimeISO().toString(),
            latest_commit: spawnSync("git", ["rev-parse", "--short", "HEAD"], {
                encoding: "utf-8",
            }).stdout.trim(),
            symbol: "🪟",
            repo: package_json.repository.url,
            releases: package_json.pane.releases,
            recent_commits: spawnSync(
                "git",
                ["log", "-5", "--pretty=format:%h%x00%an%x00%aI%x00%s%x00"],
                {
                    encoding: "utf-8",
                },
            )
                .stdout.split("\n")
                .filter(Boolean)
                .map((line) => {
                    const [commit, author, date, message] = line.split("\x00");
                    return { commit, author, date, message };
                }),
        },
    },
};

// https://docs.github.com/en/rest/guides/scripting-with-the-rest-api-and-javascript?apiVersion=2022-11-28
if (process.env.CI) {
    const octokit = new Octokit({ auth: process.env.GH_TOKEN });
    await octokit.repos.createDispatchEvent({
        owner: package_json.pane.owner,
        repo: package_json.pane.web_repo,
        ...payload,
    });
} else {
    console.dir(
        {
            ...payload,
        },
        { depth: null },
    );
}
