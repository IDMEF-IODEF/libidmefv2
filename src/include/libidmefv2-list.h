/*****
*
* Copyright (C) 2005-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libidmefv2-ids.com>
*
* This file is part of the LibIdemfv2 library.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*****/

/*
 * This is a blind rewrite of the kernel linked list handling code,
 * done so that we can dual-license libidmefv2. The code still look
 * pretty similar, but there is no way to write such list implementation
 * in many different manner.
 */

#ifndef HAVE_LIBIDMEFV2_LIBIDMEFV2_LIST_H
#define HAVE_LIBIDMEFV2_LIBIDMEFV2_LIST_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "libidmefv2-inttypes.h"

#ifdef __cplusplus
  extern "C" {
#endif

#define LIBIDMEFV2_LIST(item) libidmefv2_list_t (item) = { &(item), &(item) }


typedef struct libidmefv2_list {
        struct libidmefv2_list *next;
        struct libidmefv2_list *prev;
} libidmefv2_list_t;



static inline void __libidmefv2_list_splice(const libidmefv2_list_t *list, libidmefv2_list_t *prev, libidmefv2_list_t *next)
{
         libidmefv2_list_t *first = list->next, *last = list->prev;

         first->prev = prev;
         prev->next = first;

         last->next = next;
         next->prev = last;
}



static inline void __libidmefv2_list_add(libidmefv2_list_t *item, libidmefv2_list_t *prev, libidmefv2_list_t *next)
{
        prev->next = item;
        item->prev = prev;
        item->next = next;
        next->prev = item;
}


/**
 * libidmefv2_list_init:
 * @item: Pointer to a #libidmefv2_list_t object.
 *
 * Initialize @item. Note that this is only required if @item
 * is the head of a list, but might also be useful in case you
 * want to use libidmefv2_list_del_init().
 */
static inline void libidmefv2_list_init(libidmefv2_list_t *item)
{
        item->next = item->prev = item;
}



/**
 * libidmefv2_list_is_empty:
 * @item: Pointer to a #libidmefv2_list_t object.
 *
 * Check whether @item is empty or not.
 *
 * Returns: TRUE if @item is empty, FALSE otherwise.
 */
static inline libidmefv2_bool_t libidmefv2_list_is_empty(libidmefv2_list_t *item)
{
        return (item->next == item) ? LIBIDMEFV2_BOOL_TRUE : LIBIDMEFV2_BOOL_FALSE;
}



/**
 * libidmefv2_list_splice_tail:
 * @head: Pointer to a #libidmefv2_list_t list.
 * @list: Pointer to a #libidmefv2_list_t object to add to @head.
 *
 * All item from list @list are added at the beginning of @head.
 */
static inline void libidmefv2_list_splice(libidmefv2_list_t *head, libidmefv2_list_t *list)
{
        if ( ! libidmefv2_list_is_empty(list) )
                 __libidmefv2_list_splice(list, head, head->next);
}



/**
 * libidmefv2_list_splice_tail:
 * @head: Pointer to a #libidmefv2_list_t list.
 * @list: Pointer to a #libidmefv2_list_t object to add to @head.
 *
 * All item from list @list are added at the end of @head.
 */
static inline void libidmefv2_list_splice_tail(libidmefv2_list_t *head, libidmefv2_list_t *list)
{
        if ( ! libidmefv2_list_is_empty(list) )
                 __libidmefv2_list_splice(list, head->prev, head);
}



/**
 * libidmefv2_list_add:
 * @head: Pointer to a #libidmefv2_list_t list.
 * @item: Pointer to a #libidmefv2_list_t object to add to @head.
 *
 * Add @item at the beginning of @head list.
 */
static inline void libidmefv2_list_add(libidmefv2_list_t *head, libidmefv2_list_t *item)
{
        __libidmefv2_list_add(item, head, head->next);
}



/**
 * libidmefv2_list_add_tail:
 * @head: Pointer to a #libidmefv2_list_t list.
 * @item: Pointer to a #libidmefv2_list_t object to add to @head.
 *
 * Add @item at the tail of @head list.
 */
static inline void libidmefv2_list_add_tail(libidmefv2_list_t *head, libidmefv2_list_t *item)
{
        __libidmefv2_list_add(item, head->prev, head);
}



/**
 * libidmefv2_list_del:
 * @item: Pointer to a #libidmefv2_list_t object.
 *
 * Delete @item from the list it is linked in.
 */
static inline void libidmefv2_list_del(libidmefv2_list_t *item)
{
        item->prev->next = item->next;
        item->next->prev = item->prev;
}



/**
 * libidmefv2_list_del_init:
 * @item: Pointer to a #libidmefv2_list_t object.
 *
 * Delete @item from the list it is linked in, and reinitialize
 * @item member so that the list can be considered as empty.
 */
static inline void libidmefv2_list_del_init(libidmefv2_list_t *item)
{
        item->prev->next = item->next;
        item->next->prev = item->prev;
        libidmefv2_list_init(item);
}


/**
 * libidmefv2_list_entry:
 * @item: Pointer to a #libidmefv2_list_t object to retrieve the entry from.
 * @type: Type of the entry to retrieve.
 * @member: List member in @type used to link it to a list.
 *
 * Retrieve the entry of type @type from the #libidmefv2_list_t object @tmp,
 * using the item list member @member. Returns the entry associated with @item.
 */
#define libidmefv2_list_entry(item, type, member)                             \
        (type *) ((unsigned long) item - (unsigned long) &((type *) 0)->member)



/**
 * libidmefv2_list_for_each:
 * @list: Pointer to a #libidmefv2_list_t list.
 * @pos: Pointer to a #libidmefv2_list_t object pointing to the current list member.
 *
 * Iterate through all @list entry. libidmefv2_list_entry() can be used to retrieve
 * and entry from the @pos pointer. It is not safe to call libidmefv2_list_del() while
 * iterating using this function, see libidmefv2_list_for_each_safe().
 */
#define libidmefv2_list_for_each(list, pos)                                   \
        for ( (pos) = (list)->next; (pos) != (list); (pos) = (pos)->next )


/**
 * libidmefv2_list_for_each_safe:
 * @list: Pointer to a #libidmefv2_list_t list.
 * @pos: Pointer to a #libidmefv2_list_t object pointing to the current list member.
 * @bkp: Pointer to a #libidmefv2_list_t object pointing to the next list member.
 *
 * Iterate through all @list entry. libidmefv2_list_entry() can be used to retrieve
 * and entry from the @pos pointer. Calling libidmefv2_list_del() while iterating the
 * list is safe.
 */
#define libidmefv2_list_for_each_safe(list, pos, bkp)                         \
        for ( (pos) = (list)->next, (bkp) = (pos)->next; (pos) != (list); (pos) = (bkp), (bkp) = (pos)->next )



/**
 * libidmefv2_list_for_each_reversed:
 * @list: Pointer to a #libidmefv2_list_t list.
 * @pos: Pointer to a #libidmefv2_list_t object pointing to the current list member.
 *
 * Iterate through all @list entry in reverse order. libidmefv2_list_entry() can be
 * used to retrieve and entry from the @pos pointer. It is not safe to call
 * libidmefv2_list_del() while iterating using this function, see
 * libidmefv2_list_for_each_reversed_safe().
 */
#define libidmefv2_list_for_each_reversed(list, pos)                          \
        for ( (pos) = (list)->prev; (pos) != (list); (pos) = (pos)->prev )



/**
 * libidmefv2_list_for_each_reversed_safe:
 * @list: Pointer to a #libidmefv2_list_t list.
 * @pos: Pointer to a #libidmefv2_list_t object pointing to the current list member.
 * @bkp: Pointer to a #libidmefv2_list_t object pointing to the next list member.
 *
 * Iterate through all @list entry in reverse order. libidmefv2_list_entry() can be used to retrieve
 * and entry from the @pos pointer. Calling libidmefv2_list_del() while iterating the
 * list is safe.
 */
#define libidmefv2_list_for_each_reversed_safe(list, pos, bkp)                \
        for ( (pos) = (list)->prev, (bkp) = (pos)->prev; (pos) != (list); (pos) = (bkp), (bkp) = (pos)->prev )


/**
 * libidmefv2_list_for_each_continue:
 * @list: Pointer to a #libidmefv2_list_t list.
 * @pos: Pointer to a #libidmefv2_list_t object pointing to the current list member.
 *
 * Iterate through all @list entry starting from @pos if it is not NULL, or from
 * the start of @list if it is. libidmefv2_list_entry() can be used to retrieve
 * and entry from the @pos pointer. Calling libidmefv2_list_del() while iterating the
 * list is not safe.
 */
#define libidmefv2_list_for_each_continue(list, pos)                          \
        for ( (pos) = ((pos) == NULL) ? (list)->next : (pos)->next; (pos) != (list); (pos) = (pos)->next )


/**
 * libidmefv2_list_for_each_continue_safe:
 * @list: Pointer to a #libidmefv2_list_t list.
 * @pos: Pointer to a #libidmefv2_list_t object pointing to the current list member.
 * @bkp: Pointer to a #libidmefv2_list_t object pointing to the next list member.
 *
 * Iterate through all @list entry starting from @pos if it is not NULL, or from
 * the start of @list if it is. libidmefv2_list_entry() can be used to retrieve
 * and entry from the @pos pointer. Calling libidmefv2_list_del() while iterating the
 * list is safe.
 */
#define libidmefv2_list_for_each_continue_safe(list, pos, bkp)                \
        for ( (pos) = ((bkp) == NULL) ? (list)->next : (bkp); (bkp) = (pos)->next, (pos) != (list); (pos) = (bkp) )



#define libidmefv2_list_get_next(list, pos, class, member) \
        pos ? \
                ((pos)->member.next == (list)) ? NULL : \
                                libidmefv2_list_entry((pos)->member.next, class, member) \
        : \
                ((list)->next == (list)) ? NULL : \
                                libidmefv2_list_entry((list)->next, class, member)


#define libidmefv2_list_get_next_safe(list, pos, bkp, class, member)                                                                \
        pos ?                                                                                                            \
              (((pos) = bkp),                                                                                            \
               ((bkp) = (! (bkp) || (bkp)->member.next == list) ? NULL : libidmefv2_list_entry((pos)->member.next, class, member)), \
               (pos))                                                                                                    \
        :                                                                                                                \
              (((pos) = ((list)->next == list) ? NULL : libidmefv2_list_entry((list)->next, class, member)),                        \
               ((bkp) = (! (pos) ||(pos)->member.next == list ) ? NULL : libidmefv2_list_entry((pos)->member.next, class, member)), \
               (pos))


#ifdef __cplusplus
  }
#endif

#endif
