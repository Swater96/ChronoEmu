/*
 * Copyright (C) 2005-2010 MaNGOS <http://getmangos.com/>
 * Copyright (C) 2009-2010 MaNGOSZero <http://github.com/mangoszero/mangoszero/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef MANGOS_FORMULAS_H
#define MANGOS_FORMULAS_H

#include "World.h"

struct HonorScores
{
    float FX[15];
    float FY[15];
    float BRK[14];
};

namespace MaNGOS
{
    namespace Honor
    {
        inline float hk_honor_at_level(uint32 level, uint32 count=1)
        {
            return (float)ceil(count*(-0.53177f + 0.59357f * exp((level +23.54042f) / 26.07859f )));
        }

        inline HonorRankInfo CalculateRankInfo(HonorRankInfo prk)
        {
            prk.maxRP = 2000.00f;
            prk.minRP = 0.00f;
            prk.visualRank = 0;
            
            if (prk.rank != 0)
            {
                int8 rank = prk.positive ? prk.rank - NEGATIVE_HONOR_RANK_COUNT -1 : prk.rank - NEGATIVE_HONOR_RANK_COUNT;
                prk.maxRP = (rank)*5000.00f;
                if (prk.maxRP < 0) // in negative rank case
                    prk.maxRP *= -1;
                prk.minRP = prk.maxRP > 5000.0f ? prk.maxRP  - 5000.00f : 2000.00f;

                prk.visualRank = prk.rank > NEGATIVE_HONOR_RANK_COUNT ? prk.rank - NEGATIVE_HONOR_RANK_COUNT : prk.rank * -1;
            }

            return prk;
        }

        //What is Player's rank... private, scout...
        inline HonorRankInfo CalculateHonorRank(float honor_points)
        {
            HonorRankInfo prk;
            prk.maxRP = 2000.00f;
            prk.minRP = 0.00f;
            prk.rank  = 0;
            prk.visualRank = 0;
            prk.positive = true;

            // rank none
            if (honor_points == 0)
                return prk;

            prk.positive = honor_points > 0;
            if (!prk.positive)
                honor_points *= -1;

            uint8 rCount = prk.positive ? POSITIVE_HONOR_RANK_COUNT - 2 : NEGATIVE_HONOR_RANK_COUNT;
            uint8 firstRank = prk.positive ? NEGATIVE_HONOR_RANK_COUNT + 1 : 1;

            if(honor_points < 2000.00f)
                prk.rank = prk.positive ? firstRank : NEGATIVE_HONOR_RANK_COUNT;
            else
            {
                if(honor_points > (rCount - 1)*5000.00f)
                    prk.rank = prk.positive ? HONOR_RANK_COUNT - 1 : firstRank;
                else
                {
                    prk.rank = uint32(honor_points / 5000.00f) + firstRank;
                    prk.rank = ( prk.positive ? prk.rank  + 1 : NEGATIVE_HONOR_RANK_COUNT - prk.rank );
                }
            }

            prk = CalculateRankInfo(prk);

            return prk;
        }

        inline HonorScores GenerateScores(HonorStandingList standingList,uint32 team)
        {
            HonorScores sc;

            // initialize the breakpoint values
            sc.BRK[13] = 0.002f;
            sc.BRK[12] = 0.007f;
            sc.BRK[11] = 0.017f;
            sc.BRK[10] = 0.037f;
            sc.BRK[ 9] = 0.077f;
            sc.BRK[ 8] = 0.137f;
            sc.BRK[ 7] = 0.207f;
            sc.BRK[ 6] = 0.287f;
            sc.BRK[ 5] = 0.377f;
            sc.BRK[ 4] = 0.477f;
            sc.BRK[ 3] = 0.587f;
            sc.BRK[ 2] = 0.715f;
            sc.BRK[ 1] = 0.858f;
            sc.BRK[ 0] = 1.000f;

            // get the WS scores at the top of each break point
            for (uint8 group=0; group<14; group++)
              sc.BRK[group] = floor( (sc.BRK[group] * standingList.size()) + 0.5f);

            // initialize RP array
            // set the low point
            sc.FY[ 0] = 0;

            // the Y values for each breakpoint are fixed
            sc.FY[ 1] = 400;
            for (uint8 i=2;i<=13;i++) {
              sc.FY[i] = (i-1) * 1000;
            }

            // and finally
            sc.FY[14] = 13000;   // ... gets 13000 RP

            // the X values for each breakpoint are found from the CP scores
            // of the players around that point in the WS scores
            HonorStanding *tempSt;
            float honor;

            // initialize CP array
            sc.FX[ 0] = 0;

            for (uint8 i=1;i<=13;i++) {
              honor = 0.0f;
              tempSt = objmgr.GetHonorStandingByPosition( sc.BRK[i],team );
              if (tempSt)
              {
                  honor += tempSt->honorPoints;
                  tempSt = objmgr.GetHonorStandingByPosition( sc.BRK[i]+1, team );
                  if (tempSt)
                      honor += tempSt->honorPoints;
              }

              sc.FX[i] = honor ? honor / 2 : 0;
            }

            // set the high point if FX full filled before
            sc.FX[14] = sc.FX[13] ? standingList.begin()->honorPoints : 0;   // top scorer

            return sc;
        }

        inline float CalculateRpEarning(float CP,HonorScores sc)
        {
            // search the function for the two points that bound the given CP
            uint8 i = 0;
            while (i<14 && sc.BRK[i] > 0 && sc.FX[i] <= CP)
                i++;

            // we now have i such that FX[i] > CP >= FX[i-1]
            // so interpolate
            return (sc.FY[i] - sc.FY[i-1]) * (CP - sc.FX[i-1]) / (sc.FX[i] - sc.FX[i-1]) + sc.FY[i-1];
        }

        inline float CalculateRpDecay(float rpEarning,float RP)
        {
            float Decay = floor( (0.2f * RP) + 0.5f);
            float Delta = rpEarning - Decay;
            if (Delta < 0) {
               Delta = Delta / 2;
            }
            if (Delta < -2500) {
               Delta = -2500;
            }
            return RP + Delta;
        }

        inline float DishonorableKillPoints(int level)
        {
            float result = 10;
            if(level >= 30 && level <= 35)
                result = result + 1.5 * (level - 29);
            if(level >= 36 && level <= 41)
                result = result + 9 + 2 * (level - 35);
            if(level >= 42 && level <= 50)
                result = result + 21 + 3.2 * (level - 41);
            if(level >= 51)
                result = result + 50 + 4 * (level - 50);
            if(result > 100)
                return 100.0;
            else
                return result;
        }

        inline float HonorableKillPoints( Player *killer, Player *victim, uint32 groupsize)
        {
            if (!killer || !victim || !groupsize)
                return 0.0;

            uint32 today = sWorld.GetDateToday();

            int total_kills  = killer->CalculateTotalKills(victim,today,today);
            //int k_rank       = killer->CalculateHonorRank( killer->GetTotalHonor() );
            uint32 v_rank    = victim->GetHonorRankInfo().visualRank;
            uint32 k_level   = killer->getLevel();
            //int v_level      = victim->getLevel();
            float diff_honor = (victim->GetRankPoints() /(killer->GetRankPoints()+1))+1;
            float diff_level = (victim->getLevel()*(1.0/( killer->getLevel() )));

            int f = (10 - total_kills) >= 0 ? (10 - total_kills) : 0;
            int honor_points = int(((float)(f * 0.25)*(float)((k_level+(v_rank*5+1))*(1+0.05*diff_honor)*diff_level)));
            return (honor_points <= 400 ? honor_points : 400) / groupsize;
        }

    }
}
#endif
