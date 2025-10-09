/class="titlelink"/ {
    s/.*<a href="\([^"]*\)"[^>]*class="titlelink"[^>]*>\([^<]*\)<\/a>.*/**\2**\
\1\
/
    p
}
