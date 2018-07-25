
struct request
{
    struct list_head queuelist;     /*链表结构*/
    struct request_queue *q;        /*请求队列*/
    sector_t sector;                /*要传送的下一个扇区*/
    sector_t hard_sector;           /*要完成的下一个扇区*/
    unsigned long nr_sectors;       /*要传送的扇区数目*/
    unsigned long hard_nr_sectors;  /*要完成的扇区数目*/
    unsigned int current_nr_sectors;/*当前要传送的扇区数目*/
    unsigned int hard_cur_sectors;  /*当前要完成的扇区数目*/
    struct bio *bio;                /*请求的bio结构体的链表*/
    struct bio *biotail;            /*请求的bio结构体的链表尾*/
    void *elevator_private;
    void *elevator_private2;
    struct gendisk *rq_disk;
    unsigned long start_time;
    unsigned short nr_phys_segments;/*请求在物理内存中占据不连续段的数目*/
    unsigned short ioprio;
    char *buffer;                   /*传送的缓冲区*/
    int tag;
    int errors;
    int ref_count;                  /*引用计数*/
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
    unsigned long nr_requests;       /*最大的请求数目*/
    unsigned int nr_congestion_on;
    unsigned int nr_congestion_off;
    unsigned int nr_batching;
    unsigned int max_sectors;        /*最大的扇区数目*/
    unsigned int max_hw_sectors;
    unsigned short max_phys_segments;/*最大的段数目*/
    unsigned short max_hw_segments;
    unsigned short hardsect_size;    /*扇区尺寸大小*/
    unsigned int max_segment_size;   /*最大的段尺寸大小*/
    unsigned long seg_boundary_mask; /*段边界掩码*/
    void *dma_drain_buffer;
    unsigned int dma_drain_size;
    unsigned int dma_pad_mask;
    unsigned int dma_alignment;      /*DMA传输内存对齐*/
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
    sector_t bi_sector;            /*要传送的第一个扇区*/
    struct bio *bi_next;           /*下一个bio*/
    struct block_device *bi_bdev;
    unsigned long bi_flags;        /*状态、命令等*/
    unsigned long bi_rw;           /*低位表示READ/WRITE，高位表示优先级*/
    unsigned short bi_vcnt;        /*bio_vec的数量*/
    unsigned short bi_idx;         /*当前bvl_vec的索引*/
    unsigned int bi_phys_segments; /*不相邻物理段的数目*/
    unsigned int bi_size;          /*以字节为单位所需传送的数据大小*/
    unsigned int bi_seg_front_size;/*为了明确硬件尺寸，需要考虑bio中第一个和最后一个虚拟的可合并段的尺寸大小*/
    unsigned int bi_seg_back_size;
    unsigned int bi_max_vecs;      /*支持最大bvl_vecs的数量*/
    struct bio_vec *bi_io_vec;     /*vec列表*/
    .
    .
    .
};


struct bio_vec 
{
    struct page *bv_page;   /*页指针*/
    unsigned int bv_len;    /*传输的字节数*/
    unsigned int bv_offset; /*偏移位置*/
};


struct gendisk 
{
    int major;            /*主设备号*/
    int first_minor;      /*第一个次设备号*/
    int minors;           /*最大的次设备号，如果不能分区则为1*/
    char disk_name[DISK_NAME_LEN];  /*设备名称*/
    struct disk_part_tbl *part_tbl; /*磁盘上的分区表信息*/
    struct hd_struct part0;
    struct block_device_operations *fops;/*块设备对底层硬件的操作结构体指针*/
    struct request_queue *queue; /*请求队列*/
    void *private_data;          /*私有数据*/
    .
    .
    .
};