#ifndef _AG7240_PHY_H
#define _AG7240_PHY_H

static inline void ag7240_phy_setup(int unit)
{
    {
        athrs26_phy_setup(unit);
    }
}

static inline void ag7240_phy_link(int unit, int *link)
{
    {
         *link = athrs26_phy_is_up(unit);
    }
}

static inline void ag7240_phy_duplex(int unit, int *duplex)
{
    {
        *duplex = athrs26_phy_is_fdx(unit);
    }
}

static inline void ag7240_phy_speed(int unit, int *speed)
{
    {
        *speed = athrs26_phy_speed(unit);
    }
}

#endif /*_AG7240_PHY_H*/
