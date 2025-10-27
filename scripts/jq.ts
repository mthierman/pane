import { execSync } from "node:child_process";

const git_output = execSync(`git log -5 --pretty=format:%h%x00%an%x00%aI%x00%s%x00`, {
    encoding: "utf-8",
});

const commits = git_output
    .split("\n")
    .filter(Boolean)
    .map((line) => {
        const [commit, author, date, message] = line.split("\x00");
        return { commit, author, date, message };
    });

console.log(commits);
// console.log(JSON.stringify(commits, null, 2));
