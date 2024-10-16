#ifndef SEMA_H
#define SEMA_H

#ifdef __cplusplus
extern "C" {
#endif

extern void *semcreate  (int, int);
extern void  semdestroy (void *);
extern int   semreset   (void *, int);
extern int   semacquire (void *);
extern int   semrelease (void *);

#ifdef __cplusplus
}
#endif

#endif
