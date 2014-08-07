#include <stdlib.h>
#include <string.h>

#include "shadowdive/internal/reader.h"
#include "shadowdive/internal/writer.h"
#include "shadowdive/error.h"
#include "shadowdive/score.h"

int sd_score_create(sd_score *score) {
    if(score == NULL) {
        return SD_INVALID_INPUT;
    }
    memset(score, 0, sizeof(sd_score));
    return SD_SUCCESS;
}

void sd_score_free(sd_score *score) {
}

int sd_score_load(sd_score *score, const char *filename) {
    if(score == NULL || filename == NULL) {
        return SD_INVALID_INPUT;
    }

    sd_reader *r = sd_reader_open(filename);
    if(!r) {
        return SD_FILE_OPEN_ERROR;
    }
    
    for(int i = 0;i < sizeof(score->scores)/sizeof(score->scores[0]);i++) {
        for(int j = 0;j < sizeof(score->scores[0])/sizeof(score->scores[0][0]);j++) {
            sd_score_entry *e = &score->scores[i][j];
            e->score = sd_read_udword(r);
            sd_read_buf(r, e->name, sizeof(e->name));
            uint32_t id = sd_read_udword(r);
            e->har_id   = id&0x3F;
            e->pilot_id = (id>>6)&0x3F;
            e->padding  = (id>>12)&0xFFFFF;
            if(!sd_reader_ok(r)) {
                goto read_error;
            }
        }
    }

    sd_reader_close(r);
    return SD_SUCCESS;

read_error:
    sd_reader_close(r);
    return SD_FILE_PARSE_ERROR;
}

int sd_score_save(sd_score *score, const char *filename) {
    if(score == NULL || filename == NULL) {
        return SD_INVALID_INPUT;
    }

    sd_writer *w = sd_writer_open(filename);
    if(!w) {
        return SD_FILE_OPEN_ERROR;
    }

    for(int i = 0;i < sizeof(score->scores)/sizeof(score->scores[0]);i++) {
        for(int j = 0;j < sizeof(score->scores[0])/sizeof(score->scores[0][0]);j++) {
            sd_score_entry *e = &score->scores[i][j];
            sd_write_udword(w, e->score);
            sd_write_buf(w, e->name, sizeof(e->name));
            sd_write_udword(w, (e->har_id&0x3F) | ((e->pilot_id&0x3F)<<6) | ((e->padding&0xFFFFF)<<12));
        }
    }

    sd_writer_close(w);
    return SD_SUCCESS;
}
