#include "lib.h"

int main(void)
{
  html_header();

  printf("<html>");
  printf("<head>");
  printf("<title>Private Notes</title>");
  printf("</head>");

  printf("<body>");

  printf("<h1>Private Notes</h1>");

  printf("<p>CGI is running successfully.</p>");

  printf("</body>");
  printf("</html>");

  html_footer();

    return 0;
}