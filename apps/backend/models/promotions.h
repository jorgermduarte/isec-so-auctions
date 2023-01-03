
struct Promotions {
    int id;
    char category [50];
    int time; // in seconds
    int value; // % of promotion
    int valid; // 0 - invalid, 1 - valid
    struct Promotions *next;
    struct Promotions *prev;
};

struct Promotions* getTailPromotions(struct Promotions *head);
struct Promotions* getHeadPromotions(struct Promotions *tail);
struct Promotions* getPromotionById(struct Promotions *head, int id);
void addPromotion(struct Promotions *head, struct Promotions *promotion);
void removePromotion(struct Promotions *head, int id);
void updatePromotion(struct Promotions *head, int id, struct Promotions *promotion);
