
struct request
{
    struct list_head queuelist;     /*����ṹ*/
    struct request_queue *q;        /*�������*/
    sector_t sector;                /*Ҫ���͵���һ������*/
    sector_t hard_sector;           /*Ҫ��ɵ���һ������*/
    unsigned long nr_sectors;       /*Ҫ���͵�������Ŀ*/
    unsigned long hard_nr_sectors;  /*Ҫ��ɵ�������Ŀ*/
    unsigned int current_nr_sectors;/*��ǰҪ���͵�������Ŀ*/
    unsigned int hard_cur_sectors;  /*��ǰҪ��ɵ�������Ŀ*/
    struct bio *bio;                /*�����bio�ṹ�������*/
    struct bio *biotail;            /*�����bio�ṹ�������β*/
    void *elevator_private;
    void *elevator_private2;
    struct gendisk *rq_disk;
    unsigned long start_time;
    unsigned short nr_phys_segments;/*�����������ڴ���ռ�ݲ������ε���Ŀ*/
    unsigned short ioprio;
    char *buffer;                   /*���͵Ļ�����*/
    int tag;
    int errors;
    int ref_count;                  /*���ü���*/
    .
    .
    .
};



struct request_queue
{
    .
    .
    .
    struct list_head queue_head;
    unsigned long nr_requests;       /*����������Ŀ*/
    unsigned int nr_congestion_on;
    unsigned int nr_congestion_off;
    unsigned int nr_batching;
    unsigned int max_sectors;        /*����������Ŀ*/
    unsigned int max_hw_sectors;
    unsigned short max_phys_segments;/*���Ķ���Ŀ*/
    unsigned short max_hw_segments;
    unsigned short hardsect_size;    /*�����ߴ��С*/
    unsigned int max_segment_size;   /*���Ķγߴ��С*/
    unsigned long seg_boundary_mask; /*�α߽�����*/
    void *dma_drain_buffer;
    unsigned int dma_drain_size;
    unsigned int dma_pad_mask;
    unsigned int dma_alignment;      /*DMA�����ڴ����*/
    struct blk_queue_tag *queue_tags;
    struct list_head tag_busy_list;
    unsigned int nr_sorted;
    unsigned int in_flight;
    unsigned int rq_timeout;
    struct timer_list timeout;
    struct list_head timeout_list;
    .
    .
    .
};


struct bio 
{
    sector_t bi_sector;            /*Ҫ���͵ĵ�һ������*/
    struct bio *bi_next;           /*��һ��bio*/
    struct block_device *bi_bdev;
    unsigned long bi_flags;        /*״̬�������*/
    unsigned long bi_rw;           /*��λ��ʾREAD/WRITE����λ��ʾ���ȼ�*/
    unsigned short bi_vcnt;        /*bio_vec������*/
    unsigned short bi_idx;         /*��ǰbvl_vec������*/
    unsigned int bi_phys_segments; /*����������ε���Ŀ*/
    unsigned int bi_size;          /*���ֽ�Ϊ��λ���贫�͵����ݴ�С*/
    unsigned int bi_seg_front_size;/*Ϊ����ȷӲ���ߴ磬��Ҫ����bio�е�һ�������һ������Ŀɺϲ��εĳߴ��С*/
    unsigned int bi_seg_back_size;
    unsigned int bi_max_vecs;      /*֧�����bvl_vecs������*/
    struct bio_vec *bi_io_vec;     /*vec�б�*/
    .
    .
    .
};


struct bio_vec 
{
    struct page *bv_page;   /*ҳָ��*/
    unsigned int bv_len;    /*������ֽ���*/
    unsigned int bv_offset; /*ƫ��λ��*/
};


struct gendisk 
{
    int major;            /*���豸��*/
    int first_minor;      /*��һ�����豸��*/
    int minors;           /*���Ĵ��豸�ţ�������ܷ�����Ϊ1*/
    char disk_name[DISK_NAME_LEN];  /*�豸����*/
    struct disk_part_tbl *part_tbl; /*�����ϵķ�������Ϣ*/
    struct hd_struct part0;
    struct block_device_operations *fops;/*���豸�Եײ�Ӳ���Ĳ����ṹ��ָ��*/
    struct request_queue *queue; /*�������*/
    void *private_data;          /*˽������*/
    .
    .
    .
};