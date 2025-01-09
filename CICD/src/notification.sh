TELEGRAM_BOT_TOKEN=7377622596:AAEeuRSRTIEf0p3DLwQgL7h5LieCpwFpgiY
TIME=10
TELEGRAM_USER_ID=761683624

URL="https://api.telegram.org/bot$TELEGRAM_BOT_TOKEN/sendMessage"
TEXT="Deploy status: $CI_JOB_NAME $CI_JOB_STATUS%0AProject:+$CI_PROJECT_NAME%0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/%0ABranch:+$CI_COMMIT_REF_SLUG"

curl -s --max-time $TIME -d "chat_id=$TELEGRAM_USER_ID&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null