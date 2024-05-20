#include <stdio.h>
#include <stdint.h>
#include "smalloc.h"
int main()
{

    printf("\n");
    printf("-------------------------smalloc---------------------------- \n");
    printf("\n");
    void *p1 = smalloc(3000);
    void *p2 = smalloc(2000);
    void *p3 = smalloc(4000);
    void *p4 = smalloc(1000);

    smdump();

  /*  printf("\n");
    printf("------------------------smalloc_free------------------------------- \n");
    printf("\n");

    sfree(p2);
    sfree(p4);
    sfree(p1);
    sfree(p3);

    smdump();
*/
    printf("\n");
    printf("------------------------scenario1-------------------------------- \n");
    printf("\n");
    printf("\n");
    printf("------------------smalloc_mode(24,firtfit)---------------------------- \n");
    printf("\n");

    void *first1 = smalloc_mode(24, firstfit);
    smdump();
    sfree(first1);

    printf("\n");
    printf("------------------smalloc_mode(24,bestfit)---------------------------- \n");
    printf("\n");
    void *best1 = smalloc_mode(24, bestfit);
    smdump();
    sfree(best1);

    printf("------------------smalloc_mode(24,worstfit)---------------------------- \n");
    printf("\n");

    void *worst2 = smalloc_mode(24, worstfit);
    smdump();

    sfree(worst2);



    printf("\n");
    printf("------------------------scenario2-------------------------------- \n");
    printf("\n");
    printf("\n");

    sfree(p1);
    smdump();

    printf("------------------smalloc_mode(1000,firtfit)---------------------------- \n");
    printf("\n");

    void *first3 = smalloc_mode(1000, firstfit);
    smdump();
    sfree(first3);

    printf("\n");
    printf("------------------smalloc_mode(1000,bestfit)---------------------------- \n");
    printf("\n");
    void *best3 = smalloc_mode(1000, bestfit);
    smdump();
    sfree(best3);

    printf("------------------smalloc_mode(1000,worstfit)---------------------------- \n");
    printf("\n");

    void *worst3 = smalloc_mode(1000, worstfit);
    smdump();

    sfree(worst3);

    sfree(p4);




    return 0;
}
