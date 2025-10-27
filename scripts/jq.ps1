# https://til.simonwillison.net/jq/git-log-json

Get-ChildItem

# if (-not (Test-Path "build/generated"))
# {
#   New-Item -ItemType Directory -Path "build/generated" | Out-Null
# }

# git log -5 --pretty=format:'%h%x00%an%x00%aI%x00%s%x00' | `
#     jq -R -s '[split("\n")[:-1] | map(split("\u0000")) | .[] | {
#     "commit": .[0],
#     "author": .[1],
#     "date": .[2],
#     "message": .[3]
#   }]' > "build/generated/five_latest_commits.json"
