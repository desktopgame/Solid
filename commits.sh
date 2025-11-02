git log --no-merges --date=short --pretty=%ad \
| sort -u \
| wc -l
