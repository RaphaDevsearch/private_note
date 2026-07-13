#include "lib.h"

int main(void)
{
  html_header();

  printf("<html>");
  printf("<head>");
  printf("<title>Private Notes</title>");
  printf("</head>");

  printf("<body>");

  print_request_info();

  printf("</body>");
  printf("</html>");

  html_footer();

    return 0;
}