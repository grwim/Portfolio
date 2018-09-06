# BEGIN high BUCKET ANALYTICS
emotion_tweet_sum = 0
emotion_proportionList_highBucket = []
anger_disgust_proportionList_highBucket = []
fear_proportion_bucketList_highBucket = []
joy_love_proportion_bucketList_highBucket = []
sadness_proportion_bucketList_highBucket = []
surprise_proportion_bucketList_highBucket = []

emotion_maxProportion_highBucket = 0
anger_disgust_maxProportion_highBucket = 0
fear_maxProportion_highBucket = 0
joy_love_maxProportion_highBucket = 0
sadness_maxProportion_highBucket = 0
surprise_maxProportion_highBucket = 0

emotion_minProportion_highBucket = 1
anger_disgust_minProportion_highBucket = 1
fear_minProportion_highBucket = 1
joy_love_minProportion_highBucket = 1
sadness_minProportion_highBucket = 1
surprise_minProportion_highBucket = 1

anger_disgust_proportion_handle = 0
fear_proportion_handle = 0
joy_love_proportion_handle = 0
sadness_proportion_handle = 0
surprise_proportion_handle = 0
none_proportion_handle = 0

totalTweets_highBucket = 0

for handle in handle_bucket_highTweets:

    totalTweets_highBucket = totalTweets_highBucket + handle_totalTweetCount[handle]

    anger_disgust_proportion = 0
    fear_proportion = 0
    joy_love_proportion = 0
    sadness_proportion = 0
    surprise_proportion = 0
    none_proportion = 0

    if handle in handleTo_numLabelledAs_anger_disgust:
        anger_disgust_proportion_handle = handleTo_numLabelledAs_anger_disgust[handle] / float( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_fear:
        fear_proportion_handle = handleTo_numLabelledAs_fear[handle] / float ( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_joy:
        joy_love_proportion_handle = handleTo_numLabelledAs_joy[handle] / float( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_sadness:
        sadness_proportion_handle = handleTo_numLabelledAs_sadness[handle] / float( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_surprise:
        surprise_proportion_handle = handleTo_numLabelledAs_surprise[handle] / float( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_none:
        none_proportion_handle = handleTo_numLabelledAs_none[handle] / float( handle_totalTweetCount[handle] )

    # proportion of tweets that have emotions for this handle
    none_proportion_handle = handleTo_numLabelledAs_none[handle] / float( handle_totalTweetCount[handle] )
    emotion_proportion_handle = 1 - none_proportion_handle
    emotion_proportionList_highBucket.append(emotion_proportion_handle)

    # anger_disgust proportion list append
    anger_disgust_proportionList_highBucket.append(anger_disgust_proportion_handle)

    # fear proportion list append
    fear_proportion_bucketList_highBucket.append(fear_proportion_handle)

    # joy_love proportion list append
    joy_love_proportion_bucketList_highBucket.append(joy_love_proportion_handle)

    # sadness proportion list append
    sadness_proportion_bucketList_highBucket.append(sadness_proportion_handle)

    # surprise proportion list append
    surprise_proportion_bucketList_highBucket.append(surprise_proportion_handle)

    # update Max and Min proportions if neccesary
    if emotion_maxProportion_highBucket < emotion_proportion_handle:
        emotion_maxProportion_highBucket = emotion_proportion_handle
    if anger_disgust_proportionList_highBucket < anger_disgust_proportion_handle:
        anger_disgust_proportionList_highBucket = anger_disgust_proportion_handle
    if fear_maxProportion_highBucket < fear_proportion_handle:
        fear_maxProportion_highBucket = fear_proportion_handle
    if joy_love_maxProportion_highBucket < joy_love_proportion_handle:
        joy_love_maxProportion_highBucket = joy_love_proportion_handle
    if sadness_maxProportion_highBucket < sadness_proportion_handle:
        sadness_maxProportion_highBucket = sadness_proportion_handle
    if surprise_maxProportion_highBucket < surprise_proportion_handle:
        surprise_maxProportion_highBucket = surprise_proportion_handle

    if emotion_minProportion_highBucket > emotion_proportion_handle:
        emotion_minProportion_highBucket = emotion_proportion_handle
    if anger_disgust_minProportion_highBucket > anger_disgust_proportion_handle:
        anger_disgust_minProportion_highBucket = anger_disgust_proportion_handle
    if fear_minProportion_highBucket > fear_proportion_handle:
        fear_minProportion_highBucket = fear_proportion_handle
    if joy_love_minProportion_highBucket > joy_love_proportion_handle:
        joy_love_minProportion_highBucket = joy_love_proportion_handle
    if sadness_minProportion_highBucket > sadness_proportion_handle:
        sadness_minProportion_highBucket = sadness_proportion_handle
    if surprise_minProportion_highBucket > surprise_proportion_handle:
        surprise_minProportion_highBucket = surprise_proportion_handle


# proportion of tweets that have emotions for this bucket
proportionSum_emotion_highBucket = 0
for proportion in emotion_proportionList_highBucket:
    proportionSum_emotion_highBucket = proportionSum_emotion_highBucket + proportion
if emotion_proportionList_highBucket:
    proportion_emotion_highBucket = proportionSum_emotion_highBucket / len(emotion_proportionList_highBucket)
else:
    proportion_emotion_highBucket = 0

# proportion of anger_disgust tweets that have emotions for this bucket
proportionSum_anger_disgust_highBucket = 0
for proportion in anger_disgust_proportionList_highBucket:
    proportionSum_anger_disgust_highBucket = proportionSum_anger_disgust_highBucket + proportion
proportion_anger_disgust_highBucket = proportionSum_anger_disgust_highBucket / len(anger_disgust_proportionList_highBucket)

# proportion of fear tweets that have emotions for this bucket
proportionSum_fear_highBucket = 0
for proportion in fear_proportion_bucketList_highBucket:
    proportionSum_fear_highBucket = proportionSum_fear_highBucket + proportion
proportion_fear_highBucket = proportionSum_fear_highBucket / len(fear_proportion_bucketList_highBucket)

# proportion of joy_love tweets that have emotions for this bucket
proportionSum_joy_love_highBucket = 0
for proportion in joy_love_proportion_bucketList_highBucket:
    proportionSum_joy_love_highBucket = proportionSum_joy_love_highBucket + proportion
proportion_joy_love_highBucket = proportionSum_joy_love_highBucket / len(joy_love_proportion_bucketList_highBucket)

# proportion of sadness tweets that have emotions for this bucket
proportionSum_sadness_highBucket = 0
for proportion in sadness_proportion_bucketList_highBucket:
    proportionSum_sadness_highBucket = proportionSum_sadness_highBucket + proportion
proportion_sadness_highBucket = proportionSum_sadness_highBucket + len(sadness_proportion_bucketList_highBucket)

# proportion of surprise tweets that have emotions for this bucket
proportionSum_surprise_highBucket = 0
for proportion in surprise_proportion_bucketList_highBucket:
    proportionSum_surprise_highBucket = proportionSum_surprise_highBucket + proportion
proportion_surprise_highBucket = proportionSum_surprise_highBucket + len(surprise_proportion_bucketList_highBucket)


# for each bucket of handles, provide
# proportion of tweets that have emotions
    # sum counts of all tweets that have emotion, then divide by total num tweets
# distrubtions of emotions for each bucket
    # proportion of tweets belonging to each emotion
# range of distrubtions for each emotion across users in a bcuket
    # for each emotion, go through all users and record max and min proportions
    #

# write out to csv
csv_Data_highBucket = []
csv_row_highBucket = [   'Number of handles in this bucket',
                                        'Number of tweets corresponding to the handles in this bucket'

                                        'Proportion of all tweets in this bucket that are labeled as emotional',
                                        'Highest proportion of tweets labeled as emotional for a handle in this bucket',
                                        'highest proportion of tweets labeled as emotional for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as anger_disgust',
                                        'Highest proportion of tweets labeled as anger_disgust for a handle in this bucket',
                                        'highest proportion of tweets labeled as anger_disgust for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as fear',
                                        'Highest proportion of tweets labeled as fear for a handle in this bucket',
                                        'highest proportion of tweets labeled as fear for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as joy_love',
                                        'Highest proportion of tweets labeled as joy_love for a handle in this bucket',
                                        'highest proportion of tweets labeled as joy_love for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as sadness',
                                        'Highest proportion of tweets labeled as sadness for a handle in this bucket',
                                        'highest proportion of tweets labeled as sadness for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as surprise',
                                        'Highest proportion of tweets labeled as surprise for a handle in this bucket',
                                        'highest proportion of tweets labeled as surprise for a handle in this bucket'
                                        ]
csv_Data_highBucket.append(csv_row_highBucket)

csv_row_highBucket = [   len(handle_bucket_highTweets),
                                        totalTweets_highBucket,

                                        proportion_emotion_highBucket,
                                        emotion_maxProportion_highBucket,
                                        emotion_minProportion_highBucket,

                                        proportion_anger_disgust_highBucket,
                                        anger_disgust_maxProportion_highBucket,
                                        anger_disgust_minProportion_highBucket,

                                        proportion_fear_highBucket,
                                        fear_maxProportion_highBucket,
                                        fear_minProportion_highBucket,

                                        proportion_joy_love_highBucket,
                                        joy_love_maxProportion_highBucket,
                                        joy_love_minProportion_highBucket,

                                        proportion_sadness_highBucket,
                                        sadness_maxProportion_highBucket,
                                        sadness_minProportion_highBucket,

                                        proportion_surprise_highBucket,
                                        surprise_maxProportion_highBucket,
                                        surprise_minProportion_highBucket]

csv_Data_highBucket.append(csv_row_highBucket)

myFile = open('print_data_highBucket.csv', 'w')
writer = csv.writer(myFile)
writer.writerows(csv_Data_highBucket)
# END high BUCKET ANALYTICS




# BEGIN high BUCKET ANALYTICS -  SUBSAMPLE of last 1/10
num_to_select = len(handle_bucket_highTweets) / 10
handle_bucket_highTweets_subset =  handle_bucket_highTweets[-num_to_select:]

emotion_tweet_sum = 0
emotion_proportionList_highBucket = []
anger_disgust_proportionList_highBucket = []
fear_proportion_bucketList_highBucket = []
joy_love_proportion_bucketList_highBucket = []
sadness_proportion_bucketList_highBucket = []
surprise_proportion_bucketList_highBucket = []

emotion_maxProportion_highBucket = 0
anger_disgust_maxProportion_highBucket = 0
fear_maxProportion_highBucket = 0
joy_love_maxProportion_highBucket = 0
sadness_maxProportion_highBucket = 0
surprise_maxProportion_highBucket = 0

emotion_minProportion_highBucket = 1
anger_disgust_minProportion_highBucket = 1
fear_minProportion_highBucket = 1
joy_love_minProportion_highBucket = 1
sadness_minProportion_highBucket = 1
surprise_minProportion_highBucket = 1

anger_disgust_proportion_handle = 0
fear_proportion_handle = 0
joy_love_proportion_handle = 0
sadness_proportion_handle = 0
surprise_proportion_handle = 0
none_proportion_handle = 0

totalTweets_highBucket = 0

for handle in handle_bucket_highTweets_subset:

    totalTweets_highBucket = totalTweets_highBucket + handle_totalTweetCount[handle]

    anger_disgust_proportion = 0
    fear_proportion = 0
    joy_love_proportion = 0
    sadness_proportion = 0
    surprise_proportion = 0
    none_proportion = 0

    if handle in handleTo_numLabelledAs_anger_disgust:
        anger_disgust_proportion_handle = handleTo_numLabelledAs_anger_disgust[handle] / float( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_fear:
        fear_proportion_handle = handleTo_numLabelledAs_fear[handle] / float ( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_joy:
        joy_love_proportion_handle = handleTo_numLabelledAs_joy[handle] / float( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_sadness:
        sadness_proportion_handle = handleTo_numLabelledAs_sadness[handle] / float( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_surprise:
        surprise_proportion_handle = handleTo_numLabelledAs_surprise[handle] / float( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_none:
        none_proportion_handle = handleTo_numLabelledAs_none[handle] / float( handle_totalTweetCount[handle] )

    # proportion of tweets that have emotions for this handle
    none_proportion_handle = handleTo_numLabelledAs_none[handle] / float( handle_totalTweetCount[handle] )
    emotion_proportion_handle = 1 - none_proportion_handle
    emotion_proportionList_highBucket.append(emotion_proportion_handle)

    # anger_disgust proportion list append
    anger_disgust_proportionList_highBucket.append(anger_disgust_proportion_handle)

    # fear proportion list append
    fear_proportion_bucketList_highBucket.append(fear_proportion_handle)

    # joy_love proportion list append
    joy_love_proportion_bucketList_highBucket.append(joy_love_proportion_handle)

    # sadness proportion list append
    sadness_proportion_bucketList_highBucket.append(sadness_proportion_handle)

    # surprise proportion list append
    surprise_proportion_bucketList_highBucket.append(surprise_proportion_handle)

    # update Max and Min proportions if neccesary
    if emotion_maxProportion_highBucket < emotion_proportion_handle:
        emotion_maxProportion_highBucket = emotion_proportion_handle
    if anger_disgust_proportionList_highBucket < anger_disgust_proportion_handle:
        anger_disgust_proportionList_highBucket = anger_disgust_proportion_handle
    if fear_maxProportion_highBucket < fear_proportion_handle:
        fear_maxProportion_highBucket = fear_proportion_handle
    if joy_love_maxProportion_highBucket < joy_love_proportion_handle:
        joy_love_maxProportion_highBucket = joy_love_proportion_handle
    if sadness_maxProportion_highBucket < sadness_proportion_handle:
        sadness_maxProportion_highBucket = sadness_proportion_handle
    if surprise_maxProportion_highBucket < surprise_proportion_handle:
        surprise_maxProportion_highBucket = surprise_proportion_handle

    if emotion_minProportion_highBucket > emotion_proportion_handle:
        emotion_minProportion_highBucket = emotion_proportion_handle
    if anger_disgust_minProportion_highBucket > anger_disgust_proportion_handle:
        anger_disgust_minProportion_highBucket = anger_disgust_proportion_handle
    if fear_minProportion_highBucket > fear_proportion_handle:
        fear_minProportion_highBucket = fear_proportion_handle
    if joy_love_minProportion_highBucket > joy_love_proportion_handle:
        joy_love_minProportion_highBucket = joy_love_proportion_handle
    if sadness_minProportion_highBucket > sadness_proportion_handle:
        sadness_minProportion_highBucket = sadness_proportion_handle
    if surprise_minProportion_highBucket > surprise_proportion_handle:
        surprise_minProportion_highBucket = surprise_proportion_handle


# proportion of tweets that have emotions for this bucket
proportionSum_emotion_highBucket = 0
for proportion in emotion_proportionList_highBucket:
    proportionSum_emotion_highBucket = proportionSum_emotion_highBucket + proportion
if emotion_proportionList_highBucket:
    proportion_emotion_highBucket = proportionSum_emotion_highBucket / len(emotion_proportionList_highBucket)
else:
    proportion_emotion_highBucket = 0

# proportion of anger_disgust tweets that have emotions for this bucket
proportionSum_anger_disgust_highBucket = 0
for proportion in anger_disgust_proportionList_highBucket:
    proportionSum_anger_disgust_highBucket = proportionSum_anger_disgust_highBucket + proportion
if anger_disgust_proportionList_highBucket:
    proportion_anger_disgust_highBucket = proportionSum_anger_disgust_highBucket / len(anger_disgust_proportionList_highBucket)
else:
    proportion_anger_disgust_highBucket = 0

# proportion of fear tweets that have emotions for this bucket
proportionSum_fear_highBucket = 0
for proportion in fear_proportion_bucketList_highBucket:
    proportionSum_fear_highBucket = proportionSum_fear_highBucket + proportion
if fear_proportion_bucketList_highBucket:
    proportion_fear_highBucket = proportionSum_fear_highBucket / len(fear_proportion_bucketList_highBucket)
else:
    proportion_fear_highBucket = 0

# proportion of joy_love tweets that have emotions for this bucket
proportionSum_joy_love_highBucket = 0
for proportion in joy_love_proportion_bucketList_highBucket:
    proportionSum_joy_love_highBucket = proportionSum_joy_love_highBucket + proportion
if joy_love_proportion_bucketList_highBucket:
    proportion_joy_love_highBucket = proportionSum_joy_love_highBucket / len(joy_love_proportion_bucketList_highBucket)
else:
    proportion_joy_love_highBucket = 0

# proportion of sadness tweets that have emotions for this bucket
proportionSum_sadness_highBucket = 0
for proportion in sadness_proportion_bucketList_highBucket:
    proportionSum_sadness_highBucket = proportionSum_sadness_highBucket + proportion
if sadness_proportion_bucketList_highBucket:
    proportion_sadness_highBucket = proportionSum_sadness_highBucket + len(sadness_proportion_bucketList_highBucket)
else:
    proportion_sadness_highBucket = 0

# proportion of surprise tweets that have emotions for this bucket
proportionSum_surprise_highBucket = 0
for proportion in surprise_proportion_bucketList_highBucket:
    proportionSum_surprise_highBucket = proportionSum_surprise_highBucket + proportion
if surprise_proportion_bucketList_highBucket:
    proportion_surprise_highBucket = proportionSum_surprise_highBucket + len(surprise_proportion_bucketList_highBucket)
else:
    proportion_surprise_highBucket = 0

# for each bucket of handles, provide
# proportion of tweets that have emotions
    # sum counts of all tweets that have emotion, then divide by total num tweets
# distrubtions of emotions for each bucket
    # proportion of tweets belonging to each emotion
# range of distrubtions for each emotion across users in a bcuket
    # for each emotion, go through all users and record max and min proportions
    #

# write out to csv
csv_Data_highBucket_subset = []
csv_row_highBucket_subset = [   'Number of handles in this bucket',
                                        'Number of tweets corresponding to the handles in this bucket'

                                        'Proportion of all tweets in this bucket that are labeled as emotional',
                                        'Highest proportion of tweets labeled as emotional for a handle in this bucket',
                                        'highest proportion of tweets labeled as emotional for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as anger_disgust',
                                        'Highest proportion of tweets labeled as anger_disgust for a handle in this bucket',
                                        'highest proportion of tweets labeled as anger_disgust for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as fear',
                                        'Highest proportion of tweets labeled as fear for a handle in this bucket',
                                        'highest proportion of tweets labeled as fear for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as joy_love',
                                        'Highest proportion of tweets labeled as joy_love for a handle in this bucket',
                                        'highest proportion of tweets labeled as joy_love for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as sadness',
                                        'Highest proportion of tweets labeled as sadness for a handle in this bucket',
                                        'highest proportion of tweets labeled as sadness for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as surprise',
                                        'Highest proportion of tweets labeled as surprise for a handle in this bucket',
                                        'highest proportion of tweets labeled as surprise for a handle in this bucket'
                                        ]
csv_Data_highBucket_subset.append(csv_row_highBucket_subset)

csv_row_highBucket_subset = [   len(handle_bucket_highTweets),
                                        totalTweets_highBucket,

                                        proportion_emotion_highBucket,
                                        emotion_maxProportion_highBucket,
                                        emotion_minProportion_highBucket,

                                        proportion_anger_disgust_highBucket,
                                        anger_disgust_maxProportion_highBucket,
                                        anger_disgust_minProportion_highBucket,

                                        proportion_fear_highBucket,
                                        fear_maxProportion_highBucket,
                                        fear_minProportion_highBucket,

                                        proportion_joy_love_highBucket,
                                        joy_love_maxProportion_highBucket,
                                        joy_love_minProportion_highBucket,

                                        proportion_sadness_highBucket,
                                        sadness_maxProportion_highBucket,
                                        sadness_minProportion_highBucket,

                                        proportion_surprise_highBucket,
                                        surprise_maxProportion_highBucket,
                                        surprise_minProportion_highBucket]

csv_Data_highBucket_subset.append(csv_row_highBucket_subset)

myFile = open('print_data_highBucket_subset.csv', 'w')
writer = csv.writer(myFile)
writer.writerows(csv_Data_highBucket_subset)

# END high BUCKET ANALYTICS - SUBSAMPLE of last 1/10





# BEGIN high BUCKET ANALYTICS - RANDOM 1/10 SUBSAMPLE

# select random 10th
num_to_select = len(handle_bucket_highTweets) / 10
handle_bucket_highTweets_RANDOMsubset = random.sample(handle_bucket_highTweets, num_to_select)

emotion_tweet_sum = 0
emotion_proportionList_highBucket = []
anger_disgust_proportionList_highBucket = []
fear_proportion_bucketList_highBucket = []
joy_love_proportion_bucketList_highBucket = []
sadness_proportion_bucketList_highBucket = []
surprise_proportion_bucketList_highBucket = []

emotion_maxProportion_highBucket = 0
anger_disgust_maxProportion_highBucket = 0
fear_maxProportion_highBucket = 0
joy_love_maxProportion_highBucket = 0
sadness_maxProportion_highBucket = 0
surprise_maxProportion_highBucket = 0

emotion_minProportion_highBucket = 1
anger_disgust_minProportion_highBucket = 1
fear_minProportion_highBucket = 1
joy_love_minProportion_highBucket = 1
sadness_minProportion_highBucket = 1
surprise_minProportion_highBucket = 1

anger_disgust_proportion_handle = 0
fear_proportion_handle = 0
joy_love_proportion_handle = 0
sadness_proportion_handle = 0
surprise_proportion_handle = 0
none_proportion_handle = 0

totalTweets_highBucket = 0

for handle in handle_bucket_highTweets_RANDOMsubset:

    totalTweets_highBucket = totalTweets_highBucket + handle_totalTweetCount[handle]

    anger_disgust_proportion = 0
    fear_proportion = 0
    joy_love_proportion = 0
    sadness_proportion = 0
    surprise_proportion = 0
    none_proportion = 0

    if handle in handleTo_numLabelledAs_anger_disgust:
        anger_disgust_proportion_handle = handleTo_numLabelledAs_anger_disgust[handle] / float( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_fear:
        fear_proportion_handle = handleTo_numLabelledAs_fear[handle] / float ( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_joy:
        joy_love_proportion_handle = handleTo_numLabelledAs_joy[handle] / float( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_sadness:
        sadness_proportion_handle = handleTo_numLabelledAs_sadness[handle] / float( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_surprise:
        surprise_proportion_handle = handleTo_numLabelledAs_surprise[handle] / float( handle_totalTweetCount[handle] )
    if handle in handleTo_numLabelledAs_none:
        none_proportion_handle = handleTo_numLabelledAs_none[handle] / float( handle_totalTweetCount[handle] )

    # proportion of tweets that have emotions for this handle
    none_proportion_handle = handleTo_numLabelledAs_none[handle] / float( handle_totalTweetCount[handle] )
    emotion_proportion_handle = 1 - none_proportion_handle
    emotion_proportionList_highBucket.append(emotion_proportion_handle)

    # anger_disgust proportion list append
    anger_disgust_proportionList_highBucket.append(anger_disgust_proportion_handle)

    # fear proportion list append
    fear_proportion_bucketList_highBucket.append(fear_proportion_handle)

    # joy_love proportion list append
    joy_love_proportion_bucketList_highBucket.append(joy_love_proportion_handle)

    # sadness proportion list append
    sadness_proportion_bucketList_highBucket.append(sadness_proportion_handle)

    # surprise proportion list append
    surprise_proportion_bucketList_highBucket.append(surprise_proportion_handle)

    # update Max and Min proportions if neccesary
    if emotion_maxProportion_highBucket < emotion_proportion_handle:
        emotion_maxProportion_highBucket = emotion_proportion_handle
    if anger_disgust_proportionList_highBucket < anger_disgust_proportion_handle:
        anger_disgust_proportionList_highBucket = anger_disgust_proportion_handle
    if fear_maxProportion_highBucket < fear_proportion_handle:
        fear_maxProportion_highBucket = fear_proportion_handle
    if joy_love_maxProportion_highBucket < joy_love_proportion_handle:
        joy_love_maxProportion_highBucket = joy_love_proportion_handle
    if sadness_maxProportion_highBucket < sadness_proportion_handle:
        sadness_maxProportion_highBucket = sadness_proportion_handle
    if surprise_maxProportion_highBucket < surprise_proportion_handle:
        surprise_maxProportion_highBucket = surprise_proportion_handle

    if emotion_minProportion_highBucket > emotion_proportion_handle:
        emotion_minProportion_highBucket = emotion_proportion_handle
    if anger_disgust_minProportion_highBucket > anger_disgust_proportion_handle:
        anger_disgust_minProportion_highBucket = anger_disgust_proportion_handle
    if fear_minProportion_highBucket > fear_proportion_handle:
        fear_minProportion_highBucket = fear_proportion_handle
    if joy_love_minProportion_highBucket > joy_love_proportion_handle:
        joy_love_minProportion_highBucket = joy_love_proportion_handle
    if sadness_minProportion_highBucket > sadness_proportion_handle:
        sadness_minProportion_highBucket = sadness_proportion_handle
    if surprise_minProportion_highBucket > surprise_proportion_handle:
        surprise_minProportion_highBucket = surprise_proportion_handle


# proportion of tweets that have emotions for this bucket
proportionSum_emotion_highBucket = 0
for proportion in emotion_proportionList_highBucket:
    proportionSum_emotion_highBucket = proportionSum_emotion_highBucket + proportion
if emotion_proportionList_highBucket:
    proportion_emotion_highBucket = proportionSum_emotion_highBucket / len(emotion_proportionList_highBucket)
else:
    proportion_emotion_highBucket = 0

# proportion of anger_disgust tweets that have emotions for this bucket
proportionSum_anger_disgust_highBucket = 0
for proportion in anger_disgust_proportionList_highBucket:
    proportionSum_anger_disgust_highBucket = proportionSum_anger_disgust_highBucket + proportion
if anger_disgust_proportionList_highBucket:
    proportion_anger_disgust_highBucket = proportionSum_anger_disgust_highBucket / len(anger_disgust_proportionList_highBucket)
else:
    proportion_anger_disgust_highBucket = 0

# proportion of fear tweets that have emotions for this bucket
proportionSum_fear_highBucket = 0
for proportion in fear_proportion_bucketList_highBucket:
    proportionSum_fear_highBucket = proportionSum_fear_highBucket + proportion
if fear_proportion_bucketList_highBucket:
    proportion_fear_highBucket = proportionSum_fear_highBucket / len(fear_proportion_bucketList_highBucket)
else:
    proportion_fear_highBucket = 0

# proportion of joy_love tweets that have emotions for this bucket
proportionSum_joy_love_highBucket = 0
for proportion in joy_love_proportion_bucketList_highBucket:
    proportionSum_joy_love_highBucket = proportionSum_joy_love_highBucket + proportion
if joy_love_proportion_bucketList_highBucket:
    proportion_joy_love_highBucket = proportionSum_joy_love_highBucket / len(joy_love_proportion_bucketList_highBucket)
else:
    proportion_joy_love_highBucket = 0

# proportion of sadness tweets that have emotions for this bucket
proportionSum_sadness_highBucket = 0
for proportion in sadness_proportion_bucketList_highBucket:
    proportionSum_sadness_highBucket = proportionSum_sadness_highBucket + proportion
if sadness_proportion_bucketList_highBucket:
    proportion_sadness_highBucket = proportionSum_sadness_highBucket + len(sadness_proportion_bucketList_highBucket)
else:
    proportion_sadness_highBucket = 0

# proportion of surprise tweets that have emotions for this bucket
proportionSum_surprise_highBucket = 0
for proportion in surprise_proportion_bucketList_highBucket:
    proportionSum_surprise_highBucket = proportionSum_surprise_highBucket + proportion
if surprise_proportion_bucketList_highBucket:
    proportion_surprise_highBucket = proportionSum_surprise_highBucket + len(surprise_proportion_bucketList_highBucket)
else:
    proportion_surprise_highBucket = 0

# for each bucket of handles, provide
# proportion of tweets that have emotions
    # sum counts of all tweets that have emotion, then divide by total num tweets
# distrubtions of emotions for each bucket
    # proportion of tweets belonging to each emotion
# range of distrubtions for each emotion across users in a bcuket
    # for each emotion, go through all users and record max and min proportions
    #

# write out to csv
csv_Data_highBucket_RANDOMsample = []
csv_row_highBucket_RANDOMsample = [   'Number of handles in this bucket',
                                        'Number of tweets corresponding to the handles in this bucket'

                                        'Proportion of all tweets in this bucket that are labeled as emotional',
                                        'Highest proportion of tweets labeled as emotional for a handle in this bucket',
                                        'highest proportion of tweets labeled as emotional for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as anger_disgust',
                                        'Highest proportion of tweets labeled as anger_disgust for a handle in this bucket',
                                        'highest proportion of tweets labeled as anger_disgust for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as fear',
                                        'Highest proportion of tweets labeled as fear for a handle in this bucket',
                                        'highest proportion of tweets labeled as fear for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as joy_love',
                                        'Highest proportion of tweets labeled as joy_love for a handle in this bucket',
                                        'highest proportion of tweets labeled as joy_love for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as sadness',
                                        'Highest proportion of tweets labeled as sadness for a handle in this bucket',
                                        'highest proportion of tweets labeled as sadness for a handle in this bucket',

                                        'Proportion of all tweets in this bucket that are labeled as surprise',
                                        'Highest proportion of tweets labeled as surprise for a handle in this bucket',
                                        'highest proportion of tweets labeled as surprise for a handle in this bucket'
                                        ]
csv_Data_highBucket_RANDOMsample.append(csv_row_highBucket)

csv_row_highBucket_RANDOMsample = [   len(handle_bucket_highTweets),
                                        totalTweets_highBucket,

                                        proportion_emotion_highBucket,
                                        emotion_maxProportion_highBucket,
                                        emotion_minProportion_highBucket,

                                        proportion_anger_disgust_highBucket,
                                        anger_disgust_maxProportion_highBucket,
                                        anger_disgust_minProportion_highBucket,

                                        proportion_fear_highBucket,
                                        fear_maxProportion_highBucket,
                                        fear_minProportion_highBucket,

                                        proportion_joy_love_highBucket,
                                        joy_love_maxProportion_highBucket,
                                        joy_love_minProportion_highBucket,

                                        proportion_sadness_highBucket,
                                        sadness_maxProportion_highBucket,
                                        sadness_minProportion_highBucket,

                                        proportion_surprise_highBucket,
                                        surprise_maxProportion_highBucket,
                                        surprise_minProportion_highBucket]

csv_Data_highBucket_RANDOMsample.append(csv_row_highBucket)

myFile = open('print_data_highBucket_RANDOMsubset.csv', 'w')
writer = csv.writer(myFile)
writer.writerows(csv_Data_highBucket_RANDOMsample)
# END high BUCKET ANALYTICS - RANDOM 1/10 SUBSAMPLE
