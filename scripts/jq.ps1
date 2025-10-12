# https://til.simonwillison.net/jq/git-log-json

git log -5 --pretty=format:'%h%x00%an%x00%aI%x00%s%x00' | `
    jq -R -s '[split("\n")[:-1] | map(split("\u0000")) | .[] | {
    "commit": .[0],
    "author": .[1],
    "date": .[2],
    "message": .[3]
  }]' | Out-File -Encoding utf8 "build/commits.json"
