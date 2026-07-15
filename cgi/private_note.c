#include "lib.h"

int main(void)
{
  char raw[MAX_LINE];
  char decoded[MAX_LINE];
  char user_id[MAX_USERNAME];
  char password[MAX_PASSWORD];
  char note[MAX_NOTE];
  char action[20];

  html_header();

  //understanding();

  get_post_data(raw, MAX_LINE);
  url_decode(raw, decoded);

  extract_field(decoded, "user_id", user_id);
  extract_field(decoded, "password", password);
  extract_field(decoded, "note",     note);
  extract_field(decoded, "action",   action);

  if (!authenticate(user_id, password))
  {
    printf(
      "<meta charset=\"UTF-8\">"
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
      "<style>"
      ":root{--paper:#EDE7D6;--paper-2:#E2D9C1;--ink:#221D13;--ink-soft:#5B5540;"
      "--line:#B9AC85;--amber:#B96A1E;--stamp:#9C3B2C;}"
      "@media (prefers-color-scheme: dark){:root{--paper:#15130C;--paper-2:#1C1810;"
      "--ink:#EDE3C6;--ink-soft:#B3A987;--line:#3A331F;--amber:#E7A23C;--stamp:#D2705B;}}"
      "*{box-sizing:border-box;}"
      "body{margin:0;min-height:100vh;display:grid;place-items:center;padding:24px;"
      "background:var(--paper);color:var(--ink);"
      "font-family:'IBM Plex Sans',-apple-system,sans-serif;}"
      ".page{width:min(100%%,440px);background:var(--paper-2);border:1px solid var(--line);"
      "border-radius:8px;overflow:hidden;}"
      ".topbar{padding:14px 16px;border-bottom:1px solid var(--line);}"
      ".brand{font-family:'JetBrains Mono',monospace;font-weight:700;font-size:0.95rem;"
      "color:var(--ink);}"
      ".brand span{color:var(--amber);}"
      ".card{padding:28px 24px 24px;}"
      ".eyebrow{font-family:'JetBrains Mono',monospace;font-size:0.78rem;"
      "letter-spacing:0.06em;color:var(--stamp);display:inline-flex;align-items:center;"
      "gap:8px;margin-bottom:8px;}"
      ".eyebrow span{color:var(--ink-soft);}"
      "h1{font-family:'JetBrains Mono',monospace;font-size:1.4rem;margin:0 0 8px;"
      "color:var(--stamp);}"
      "p{margin:0 0 18px;color:var(--ink-soft);line-height:1.55;}"
      ".footer{margin-top:4px;text-align:center;font-size:0.9rem;color:var(--ink-soft);}"
      ".footer a{color:var(--amber);text-decoration:none;}"
      "@media (max-width:480px){.card{padding:22px 18px 20px;}.topbar{padding:12px 14px;}}"
      "</style>"
      "<main class=\"page\">"
      "<div class=\"topbar\"><div class=\"brand\"><span>$</span> private_note</div></div>"
      "<div class=\"card\">"
      "<div class=\"eyebrow\"><span>$</span>access denied</div>"
      "<h1>Login failed</h1>"
      "<p>Check your user ID and password, then try again.</p>"
      "<div class=\"footer\"><a href=\"../index.html\">&larr; back to sign in</a></div>"
      "</div></main>"
    );
    html_footer();
    return 0;
  }

  const char *internal_id = find_user_id(user_id);

  if (strcmp(action, "add") == 0)
  {
    if (add_note(internal_id, note))
    {
      printf("<p>Note saved.</p>");
    }
    else
    {
      printf("<p>Could not save your note (no room left).</p>");
    }
  }

  view_notes(internal_id);

  html_footer();

  return 0;
}