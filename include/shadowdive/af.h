/*! \file 
 * \brief Contains functions for handling .AF (HAR) files.
 * \license MIT
 */ 

#ifndef _SD_AF_H
#define _SD_AF_H

#include <stdint.h>
#include "shadowdive/move.h"

#ifdef __cplusplus 
extern "C" {
#endif

#define MAX_AF_MOVES 70

typedef struct {
    uint16_t file_id;
    uint16_t unknown_a;
    uint32_t endurance;
    uint8_t unknown_b;
    uint16_t power;
    int32_t forward_speed;
    int32_t reverse_speed;
    int32_t jump_speed;
    int32_t fall_speed;
    uint8_t unknown_c;
    uint8_t unknown_d;

    sd_move *moves[MAX_AF_MOVES];
    char soundtable[30];
} sd_af_file;

/*! \brief Initialize AF container
 *
 * Initializes the AF container with empty values.
 *
 * Return values:
 * - SD_INVALID_INPUT if AF struct value was NULL.
 * - SD_SUCCESS on success. 
 *
 * \param af Allocated AF struct pointer.
 * \return SD_SUCCESS or errorcode.
 */
int sd_af_create(sd_af_file *af);

/*! \brief Copy AF structure
 *
 * Copies the contents of an AF file structure. _ALL_ internals will be copied.
 * The copied structure must be freed using sd_af_free().
 *
 * Destination buffer does not need to be cleared. Source buffer must be a valid
 * AF file structure, of problems are likely to appear.
 *
 * Return values:
 * - SD_OUT_OF_MEMORY If memory ran out. Destination struct should now be considered invalid and freed.
 * - SD_INVALID_INPUT Either input value was NULL.
 * - SD_SUCCESS on success. 
 *
 * \param dst Destination AF struct pointer.
 * \param src Source AF struct pointer.
 * \return SD_SUCCESS or errorcode.
 */
int sd_af_copy(sd_af_file *dst, const sd_af_file *src);

/*! \ brief Set move
 *
 * Sets a HAR move to a given index in a AF file structure. Move will be copied,
 * so remember to free your local copy yourself.
 *
 * Index should be between 0 and 69 (inclusive); any other values will result in error.
 *
 * A NULL value for move will result in the old move at index getting freed.
 * 
 * Return values:
 * - SD_OUT_OF_MEMORY If memory ran out. This struct should now be considered invalid and freed.
 * - SD_INVALID_INPUT Index was invalid or a pointer was NULL.
 * - SD_SUCCESS on success. 
 *
 * \param af AF struct pointer.
 * \param index Index of the move. Must be 0 <= index <= 69.
 * \param move sd_move struct. This will be copied.
 * \return SD_SUCCESS or errorcode.
 */ 
int sd_af_set_move(sd_af_file *af, int index, const sd_move *move);

/*! \ brief Get move
 * 
 * Gets a HAR move from an index in AF file structure. Index must be
 * between values 0 and 69 (inclusive); any other value will result in error.
 *
 * Return values:
 * - NULL on failure (wrong index or move does not exist)
 * - On success, a pointer to sd_move struct
 *
 * \param af AF struct pointer.
 * \param index Index of the move. Must be 0 <= index <= 69.
 * \return sd_move pointer or NULL.
 */ 
sd_move* sd_af_get_move(sd_af_file *af, int index);

/*! \brief Load AF file
 *
 * Loads the given AF file to memory. The structure must be initialized with sd_af_create() 
 * before using this function.´Loading to a previously loaded or filled sd_bk_file structure 
 * will result in old data and pointers getting lost. This is very likely to cause a memory leak.
 * 
 * Return values:
 * - SD_FILE_OPEN_ERROR If file could not be opened.
 * - SD_FILE_PARSE_ERROR If file does not contain valid data or has syntax problems.
 * - SD_OUT_OF_MEMORY If memory ran out. This struct should now be considered invalid and freed.
 * - SD_SUCCESS on success.
 *
 * \param af AF struct pointer.
 * \param filename Name of the AF file.
 * \return SD_SUCCESS or errorcode.
 */
int sd_af_load(sd_af_file *af, const char *filename);

/*! \brief Save AF file
 *
 * Saves the given AF file from memory to a file on disk. The structure must be
 * at least initialized by using sd_af_create before running this.
 *
 * Return values:
 * - SD_FILE_OPEN_ERROR If file could not be opened.
 * - SD_SUCCESS on success.
 *
 * \param af AF struct pointer.
 * \param filename Name of the AF file
 * \return SD_SUCCESS or errorcode.
 */
int sd_af_save(const sd_af_file *af, const char* filename);

/*! \brief Free AF container
 * 
 * Frees up all memory reserved by the AF container.
 * All contents will be freed, all pointers to contents will be invalid.
 *
 * \param af AF struct pointer.
 */
void sd_af_free(sd_af_file *af);

#ifdef __cplusplus
}
#endif

#endif // _SD_AF_H
